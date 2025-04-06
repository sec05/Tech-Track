// Copyright 2025 Spencer Evans-Cole

#include "HTTPSession.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../Handlers/Requests/DataRequestHandler.h"

HTTPSession::HTTPSession(boost::asio::ssl::stream<boost::beast::tcp_stream> stream, std::vector<std::string> valid_technologies,
    std::vector<std::string> valid_companies)
    : stream_(std::move(stream)), valid_technologies_(valid_technologies),
    valid_companies_(valid_companies) {
    valid_companies_.push_back("all");
}

void HTTPSession::Run() {
    // Start reading the HTTP request from the client.
    DoHandshake();
}

void HTTPSession::DoHandshake() {
    auto self = shared_from_this();
    stream_.async_handshake(boost::asio::ssl::stream_base::server, 
        [this, self](boost::beast::error_code ec) {
            if (!ec) {
                DoRead();
            } else {
                std::cerr << "SSL Handshake failed: " << ec.message() << std::endl;
            }
        });
}

void HTTPSession::DoRead() {
    auto self = shared_from_this();

    // Asynchronously read the HTTP request from the socket into req_.
    http::async_read(stream_, buffer_, req_,
        [this, self](beast::error_code ec, std::size_t bytes_transferred){
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
    // Check if the request is valid and parse the target
    if (req_.method() != http::verb::get) {
        SendBadRequest("Unsupported HTTP-method");
        return;
    } else if (req_.target().empty() || req_.target()[0] != '/') {
        SendBadRequest("Invalid target");
        return;
    } else {
        // Parse the target to extract company and technology
        // The target is expected to be in the format "/company/technology"

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

        //  handle spaces in names
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

        //  validate technology and company

        if (!IsValidTechnology(technology_)) {
            SendBadRequest("Invalid technology");
            return;
        }
        if (!IsValidCompany(company_)) {
            SendBadRequest("Invalid company");
            return;
        }

        // now pass off to the request to the data request handler
        DataRequestHandler handler = DataRequestHandler(std::move(req_), shared_from_this());

        std::unique_ptr<http::response<http::string_body>> res = handler.HandleRequest();

        DoWrite(std::move(res));
        std::cout << "Response sent" << std::endl;
        return;
    }
    SendBadRequest("Unknown request");
    return;
}

void HTTPSession::SendBadRequest(const std::string& why) {
    // Build a bad request response with the provided reason.
    http::response<http::string_body> res{ http::status::bad_request, req_.version() };
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req_.keep_alive());
    res.body() = why;
    res.prepare_payload();
    DoWrite(std::move(res));
}

bool HTTPSession::IsValidTechnology(std::string technology) {
    for (std::string valid_technology : valid_technologies_) {
        if (technology == valid_technology) {
            return true;
        }
    }
    return false;
}

bool HTTPSession::IsValidCompany(std::string company) {
    for (std::string valid_company : valid_companies_) {
        if (company == valid_company) {
            return true;
        }
    }
    return false;
}
