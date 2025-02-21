// Copyright 2025 Spencer Evans-Cole

#include "DataRequestHandler.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../../Server/HTTPSession.h"

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
    free(res_);
}

http::response<http::string_body>* DataRequestHandler::HandleRequest() {
    // Create a simple "Hello, World!" response.
    std::string body = "I am a data request handler. You requested data for " + name + ".";
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
