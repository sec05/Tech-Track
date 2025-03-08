// Copyright 2025 Spencer Evans-Cole

#include "DataRequestHandler.h"

#include <utility>
#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

#include "../../Server/HTTPSession.h"
#include "../Data/DummyDataHandler.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

DataRequestHandler::DataRequestHandler
        (http::request<http::string_body>&& req, std::shared_ptr<HTTPSession> session)
        : RequestHandler(std::move(req), std::move(session)) {
            ParseRequest();
    }

DataRequestHandler::~DataRequestHandler() {
    delete res_;
}

http::response<http::string_body>* DataRequestHandler::HandleRequest() {
    DummyDataHandler data_handler(10, 2);
    arma::vec future = data_handler.LeastSquaresPreditct(5);
    arma::vec data = data_handler.GetDataVector(1);
    boost::json::object response_json;
    boost::json::array data_json;
    boost::json::array future_json;
    for (int i = 0; i < data.n_elem; i++) {
        data_json.push_back(data(i));
    }
    for (int i = 0; i < future.n_elem; i++) {
        future_json.push_back(future(i));
    }
    response_json["data"] = data_json;
    response_json["future"] = future_json;
    std::string body = boost::json::serialize(response_json);
    res_ = new http::response<http::string_body>(http::status::ok, req_.version());
    res_->set(http::field::server, "Boost.Beast");
    res_->set(http::field::content_type, "text/plain");
    res_->keep_alive(req_.keep_alive());
    res_->body() = body;
    res_->prepare_payload();

    return res_;
}

void DataRequestHandler::ParseRequest() {
    // Parse the request to extract the name.
    std::string request = req_.target();
    name = request.substr(6);
}
