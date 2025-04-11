// Copyright 2025 Spencer Evans-Cole

#include "DataHandler.h"

#include <armadillo>
#include <armadillo>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <boost/json.hpp>
#include "HttpPoller.h"

PollingCube GetDataFromPolling() {
    using namespace arma;
    using namespace boost::json;

    // Temporary structures to hold dynamic data
    std::set<std::string> all_techs;
    std::set<std::string> all_dates;
    std::vector<std::string> endpoints;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, double>>> full_data;
    //                 ^ endpoint      ^ date        ^ tech         ^ count

    HttpPoller::instance().withResponses([&](const auto& responses) {
        for (const auto& [endpoint, body] : responses) {
            endpoints.push_back(endpoint);
            try {
                value parsed = parse(body);
                if (!parsed.is_object()) continue;
                object date_obj = parsed.get_object();

                for (auto& [date_str, tech_val] : date_obj) {
                    all_dates.insert(date_str);

                    if (!tech_val.is_object()) continue;
                    object techs = tech_val.get_object();

                    for (auto& [tech_str, count_val] : techs) {
                        all_techs.insert(tech_str);
                        full_data[endpoint][date_str][tech_str] = count_val.to_number<double>();
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "[WARN] Failed to parse JSON from " << endpoint << ": " << e.what() << "\n";
            }
        }
    });

    // Final label orderings
    std::vector<std::string> tech_list(all_techs.begin(), all_techs.end());
    std::vector<std::string> date_list(all_dates.begin(), all_dates.end());
    std::vector<std::string> endpoint_list = endpoints;

    arma::cube result(tech_list.size(), endpoint_list.size(), date_list.size(), fill::zeros);

    for (size_t e = 0; e < endpoint_list.size(); ++e) {
        const auto& endpoint = endpoint_list[e];
        for (size_t d = 0; d < date_list.size(); ++d) {
            const auto& date = date_list[d];
            const auto& tech_map = full_data[endpoint][date];
            for (size_t t = 0; t < tech_list.size(); ++t) {
                const auto& tech = tech_list[t];
                auto it = tech_map.find(tech);
                if (it != tech_map.end()) {
                    result(t, e, d) = it->second;
                }
            }
        }
    }

    return PollingCube{
        .data = std::move(result),
        .tech_labels = std::move(tech_list),
        .endpoint_labels = std::move(endpoint_list),
        .date_labels = std::move(date_list)
    };
}

DataHandler::DataHandler(int m, int n) {
    data_cube_ = FillRandomPollingCube(m, n, 10);
    
}

DataHandler::~DataHandler() {
}

PollingCube DataHandler::GetDataFromPolling() {
    return ::GetDataFromPolling();
}

PollingCube DataHandler::FillRandomPollingCube(size_t num_techs, size_t num_endpoints, size_t num_dates) {
    using namespace arma;

    cube data(num_techs, num_endpoints, num_dates);
    data.randu();  // fill with uniform [0,1)

    std::vector<std::string> techs;
    std::vector<std::string> endpoints;
    std::vector<std::string> dates;

    for (size_t i = 0; i < num_techs; ++i)
        techs.push_back("tech_" + std::to_string(i));
    for (size_t i = 0; i < num_endpoints; ++i)
        endpoints.push_back("endpoint_" + std::to_string(i));
    for (size_t i = 0; i < num_dates; ++i)
        dates.push_back("2025-04-" + std::to_string(i));

    return PollingCube{
        .data = std::move(data),
        .tech_labels = std::move(techs),
        .endpoint_labels = std::move(endpoints),
        .date_labels = std::move(dates)
    };
}

arma::mat DataHandler::GetMatrixByDateAndTech() {
    size_t num_techs = data_cube_.tech_labels.size();
    size_t num_dates = data_cube_.date_labels.size();
    size_t num_endpoints = data_cube_.endpoint_labels.size();

    arma::mat result(num_dates, num_techs, arma::fill::zeros);

    for (size_t d = 0; d < num_dates; ++d) {
        for (size_t t = 0; t < num_techs; ++t) {
            double total = 0.0;
            for (size_t e = 0; e < num_endpoints; ++e) {
                total += data_cube_.data(t, e, d);
            }
            result(d, t) = total;
        }
    }

    return result;
}

arma::vec DataHandler::GetDataByEndpointAndTech(const std::string& endpoint, const std::string& tech) {
    size_t endpoint_idx = std::distance(data_cube_.endpoint_labels.begin(), 
                                        std::find(data_cube_.endpoint_labels.begin(), data_cube_.endpoint_labels.end(), endpoint));
    size_t tech_idx = std::distance(data_cube_.tech_labels.begin(), 
                                    std::find(data_cube_.tech_labels.begin(), data_cube_.tech_labels.end(), tech));

    if (endpoint_idx >= data_cube_.endpoint_labels.size() || tech_idx >= data_cube_.tech_labels.size()) {
        tech_idx = 0;
        endpoint_idx = 0;
    }

    // Extract 1D vector over dates for (tech, endpoint)
    return data_cube_.data.tube(tech_idx, endpoint_idx);
}
