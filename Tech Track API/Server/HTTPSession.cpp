// Copyright 2025 Spencer Evans-Cole

#include "HTTPSession.h"

#include <iostream>
#include <string>
#include <utility>

#include "../Handlers/Requests/DataRequestHandler.h"

HTTPSession::HTTPSession(tcp::socket socket)
    : socket_(std::move(socket)) {
    std::cout << "New HTTP session started." << std::endl;
}

void HTTPSession::Run() {
    // Start reading the HTTP request from the client.
    DoRead();
}

void HTTPSession::DoRead() {
    auto self = shared_from_this();
    // Asynchronously read the HTTP request from the socket into req_.
    http::async_read(socket_, buffer_, req_,
                     [this, self](beast::error_code ec, std::size_t bytes_transferred) {
                         boost::ignore_unused(bytes_transferred);
                         if (!ec) {
                             // Process the request once it has been fully read.
                             HandleRequest();
                         } else {
                             std::cerr << "Read error: " << ec.message() << std::endl;
                         }
                     });
}

void HTTPSession::HandleRequest() {
    // For this example, we only support GET requests.
    if (req_.method() != http::verb::get) {
        std::cout << "Unsupported HTTP-method: " << req_.method_string() << std::endl;
        SendBadRequest("Unsupported HTTP-method");
        return;
    } else if (req_.target().empty() || req_.target()[0] != '/') {
        std::cout << "Invalid target: " << req_.target() << std::endl;
        SendBadRequest("Invalid target");
        return;
    } else if (req_.target().find("/data/") == 0) {
        std::cout << "Data request: " << req_.target() << std::endl;
    } else {
        std::cout << "Unknown request: " << req_.target() << std::endl;
        SendBadRequest("Unknown request");
        return;
    }

    DataRequestHandler handler(std::move(req_), shared_from_this());

    http::response<http::string_body>* res = handler.HandleRequest();

    DoWrite(std::move(*res));
}

void HTTPSession::SendBadRequest(const std::string &why) {
    // Build a bad request response with the provided reason.
    http::response<http::string_body> res{http::status::bad_request, req_.version()};
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req_.keep_alive());
    res.body() = why;
    res.prepare_payload();
    DoWrite(std::move(res));
}
