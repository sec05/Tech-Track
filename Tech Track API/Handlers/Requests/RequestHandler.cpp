// Copyright 2025 Spencer Evans-Cole

#include "RequestHandler.h"

#include <memory>
#include <utility>

RequestHandler::RequestHandler
    (http::request<http::string_body>&& req, std::shared_ptr<HTTPSession> session)
    : req_(std::move(req)), session_(std::move(session)) {
}

RequestHandler::~RequestHandler() {
}

http::response<http::string_body>* RequestHandler::HandleRequest() {
    return nullptr;
}

void RequestHandler::ParseRequest() {
}
