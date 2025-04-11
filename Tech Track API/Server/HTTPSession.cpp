// Copyright 2025 Spencer Evans-Cole

#include "HTTPSession.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "../Handlers/Requests/DataRequestHandler.h"

HTTPSession::HTTPSession(beast::tcp_stream stream,
    std::vector<std::string> valid_technologies,
    std::vector<std::string> valid_companies)
    : stream_(std::move(stream)),
      valid_technologies_(std::move(valid_technologies)),
      valid_companies_(std::move(valid_companies)) {
    valid_companies_.push_back("all");
}

void HTTPSession::Run() {
    DoRead();  // No SSL handshake required for HTTP
}

void HTTPSession::DoRead() {
    auto self = shared_from_this();

    http::async_read(stream_, buffer_, req_,
        [this, self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                HandleRequest();
            } else {
                std::cerr << "Read error: " << ec.message() << std::endl;
            }
        });
}

void HTTPSession::AddCORSHeaders(http::response<http::string_body>& res) {
    res.set(http::field::access_control_allow_origin, "*");
    res.set(http::field::access_control_allow_methods, "GET, OPTIONS");
    res.set(http::field::access_control_allow_headers, "Content-Type");
}

void HTTPSession::HandleRequest() {
    if (req_.method() == http::verb::options) {
        http::response<http::string_body> res{ http::status::ok, req_.version() };
        AddCORSHeaders(res);
        res.prepare_payload();
        DoWrite(std::move(res));
        return;
    } else if (req_.method() != http::verb::get) {
        SendBadRequest("Unsupported HTTP-method");
        return;
    }

    if (req_.target().empty() || req_.target()[0] != '/') {
        SendBadRequest("Invalid target");
        return;
    }

    std::string company = req_.target().substr(1);
    std::string technology = company;
    for (size_t i = 0; i < company.size(); i++) {
        if (company[i] == '/') {
            company = company.substr(0, i);
            technology = technology.substr(i + 1);
            break;
        }
    }
    company_ = company;
    technology_ = technology;

    // Decode %20 to space
    size_t pos = 0;
    while ((pos = company_.find("%20", pos)) != std::string::npos) {
        company_.replace(pos, 3, " ");
        pos += 1;
    }
    pos = 0;
    while ((pos = technology_.find("%20", pos)) != std::string::npos) {
        technology_.replace(pos, 3, " ");
        pos += 1;
    }

    if (!IsValidTechnology(technology_)) {
        SendBadRequest("Invalid technology");
        return;
    }

    if (!IsValidCompany(company_)) {
        SendBadRequest("Invalid company");
        return;
    }

    DataRequestHandler handler(std::move(req_), shared_from_this());
    auto res = handler.HandleRequest();
    AddCORSHeaders(res);
    DoWrite(std::move(res));
    std::cout << "Response sent" << std::endl;
}

void HTTPSession::SendBadRequest(const std::string& why) {
    http::response<http::string_body> res{ http::status::bad_request, req_.version() };
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req_.keep_alive());
    res.body() = why;
    AddCORSHeaders(res);
    res.prepare_payload();
    DoWrite(std::move(res));
}

bool HTTPSession::IsValidTechnology(std::string technology) {
    for (const std::string& valid_technology : valid_technologies_) {
        if (technology == valid_technology) return true;
    }
    return false;
}

bool HTTPSession::IsValidCompany(std::string company) {
    for (const std::string& valid_company : valid_companies_) {
        if (company == valid_company) return true;
    }
    return false;
}
