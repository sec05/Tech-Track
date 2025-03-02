// Copyright 2025 Spencer Evans-Cole

#include "HTTPSession.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../Handlers/Requests/DataRequestHandler.h"

HTTPSession::HTTPSession(tcp::socket socket, std::vector<std::string> valid_technologies,
    std::vector<std::string> valid_companies)
    : socket_(std::move(socket)), valid_technologies_(valid_technologies),
    valid_companies_(valid_companies) {
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
    } else if (req_.target().find("/data/technology/") == 0) {
        std::string technology = req_.target().substr(17);
        if (!IsValidTechnology(technology)) {
            std::cout << "Invalid technology: " << technology << std::endl;
            SendBadRequest("Invalid technology");
            return;
        }
    } else if (req_.target().find("/data/") == 0) {
        std::string company = req_.target().substr(6);
        std::string technology = company;
        for (int i = 0; i < company.size(); i++) {
            if (company[i] == '/') {
                company = company.substr(0, i);
                technology = technology.substr(i+1);
                break;
            }
        }
        std::cout << "Company: " << company << std::endl;
        std::cout << "Technology: " << technology << std::endl;
        if (!IsValidTechnology(technology)) {
            std::cout << "Invalid technology: " << technology << std::endl;
            SendBadRequest("Invalid technology");
            std::cout << "Valid technologies: ";
            for (std::string valid_technology : valid_technologies_) {
                std::cout << valid_technology << " ";
            }
            std::cout << std::endl;
            return;
        }
        if (!IsValidCompany(company)) {
            std::cout << "Invalid company: " << company << std::endl;
            SendBadRequest("Invalid company");
            std::cout << "Valid companies: ";
            for (std::string valid_company : valid_companies_) {
                std::cout << valid_company << " ";
            }
            std::cout << std::endl;
            return;
        }
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
