// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_HANDLERS_REQUESTS_DATAREQUESTHANDLER_H_
#define TECH_TRACK_API_HANDLERS_REQUESTS_DATAREQUESTHANDLER_H_

#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../../Server/HTTPSession.h"
#include "RequestHandler.h"
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class DataRequestHandler : public RequestHandler {
 public:
    DataRequestHandler
        (http::request<http::string_body>&& req, std::shared_ptr<HTTPSession> session);
    ~DataRequestHandler() override;
    http::response<http::string_body>* HandleRequest() override;

    void ParseRequest() override;
 private:
    std::string name;
};

#endif  // TECH_TRACK_API_HANDLERS_REQUESTS_DATAREQUESTHANDLER_H_
