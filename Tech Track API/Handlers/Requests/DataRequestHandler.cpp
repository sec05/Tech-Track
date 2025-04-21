// Copyright 2025 Spencer Evans-Cole

#include "DataRequestHandler.h"

#include <utility>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

#include "../../Server/HTTPSession.h"
#include "../Data/DummyDataHandler.h"
#include "../../Prediction/LS/LSPredictor.h"
#include "../../Prediction/LS/DummyDataGenerator.h"
#include "../../Prediction/LSTM/LSTMDriver.h"
#include "../Data/HttpPoller.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

DataRequestHandler::DataRequestHandler(http::request<http::string_body> &&req,
    std::shared_ptr<HTTPSession> session)
    : RequestHandler(std::move(req), std::move(session)), company_(""), technology_("") {
    ParseRequest();
    res_ = http::response<http::string_body>(http::status::ok, GetRequest().version());
}

DataRequestHandler::~DataRequestHandler() {
    company_ = "";
    technology_ = "";
}

http::response<http::string_body> DataRequestHandler::HandleRequest() {
    DummyDataHandler data_handler(10, 2);
    boost::json::object response_json;
    boost::json::array data_json;
    boost::json::array dates_json;
    boost::json::string date_json;
    if (company_ == "all") {
        /*arma::mat m = LSTMDriver::getInstance().predict_from_last();
        std::vector<std::string> dateLabels = data_handler.GetDateLabels();
        arma::mat data = data_handler.GetMatrixByDateAndTech();
        for(int i = 0; i < dateLabels.size(); i++)
        {
            double value = data(i, 0);
            data_json.push_back(value);
            date_json = dateLabels[i];
            dates_json.push_back(date_json);
        }
        for (int i = 0; i < m.n_rows; i++)
        {
            double value = m(i, 0);
            data_json.push_back(value);
            date_json = dateLabels[i] + "+1";
            dates_json.push_back(date_json);
        }*/
        company_ = "google";
        int tech_index = 0;
        int endpoint_index = 0;
        for (int i = 0; i < data_handler.GetTechLabels().size(); i++) {
            if (data_handler.GetTechLabels()[i] == technology_) {
                tech_index = i;
            }
        }
        for (int i = 0; i < data_handler.GetEndpointLabels().size(); i++) {
            if (data_handler.GetEndpointLabels()[i] == company_) {
                endpoint_index = i;
            }
        }
        arma::dvec data = data_handler.GetDataByEndpointAndTech(company_, technology_);
        LSPredictor predictor(data);
        arma::dvec output = predictor.Predict();
        std::vector<std::string> dateLabels = data_handler.GetDateLabels();
        for (int i = 0; i < dateLabels.size(); i++) {
            data_json.push_back(data[i]);
            date_json = dateLabels[i];
            dates_json.push_back(date_json);
        }
        for (int i = 0; i < output.n_elem; i++) {
            data_json.push_back(output[i]);
            date_json = dateLabels[i] + "+1";
            dates_json.push_back(date_json);
        }
    } else if (company_ != "all") {
        int tech_index = 0;
        int endpoint_index = 0;
        for (int i = 0; i < data_handler.GetTechLabels().size(); i++) {
            if (data_handler.GetTechLabels()[i] == technology_) {
                tech_index = i;
            }
        }
        for (int i = 0; i < data_handler.GetEndpointLabels().size(); i++) {
            if (data_handler.GetEndpointLabels()[i] == company_) {
                endpoint_index = i;
            }
        }
        arma::dvec data = data_handler.GetDataByEndpointAndTech(company_, technology_);
        LSPredictor predictor(data);
        arma::dvec output = predictor.Predict();
        std::vector<std::string> dateLabels = data_handler.GetDateLabels();
        for (int i = 0; i < dateLabels.size(); i++) {
            data_json.push_back(data[i]);
            date_json = dateLabels[i];
            dates_json.push_back(date_json);
        }
        for (int i = 0; i < output.n_elem; i++) {
            data_json.push_back(output[i]);
            date_json = dateLabels[i] + "+1";
            dates_json.push_back(date_json);
        }
    }

    response_json["values"] = data_json;
    response_json["times"] = dates_json;
    std::string body = boost::json::serialize(response_json);
    res_.set(http::field::server, "Boost.Beast");
    res_.set(http::field::content_type, "text/plain");
    res_.keep_alive(GetRequest().keep_alive());
    res_.body() = body;

    res_.prepare_payload();
    return std::move(res_);
}

void DataRequestHandler::ParseRequest() {
    // Parse the request to extract the name.
    std::string request = GetRequest().target();
    company_ = GetSession()->GetCompany();
    technology_ = GetSession()->GetTechnology();
}
