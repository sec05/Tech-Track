// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_HANDLERS_REQUESTS_REQUESTHANDLER_H_
#define TECH_TRACK_API_HANDLERS_REQUESTS_REQUESTHANDLER_H_

#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../../Server/HTTPSession.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class RequestHandler {
 protected:
    http::request<http::string_body> req_;
    std::shared_ptr<HTTPSession> session_;
    http::response<http::string_body>* res_;
 public:
    RequestHandler
      (http::request<http::string_body>&& req, std::shared_ptr<HTTPSession> session);
    virtual ~RequestHandler();
    virtual http::response<http::string_body>* HandleRequest();
    virtual void ParseRequest();
};

#endif  // TECH_TRACK_API_HANDLERS_REQUESTS_REQUESTHANDLER_H_
