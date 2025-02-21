// Copyright 2025 Spencer Evans-Cole
#include "Server.h"

#include <memory>
#include <utility>
#include <iostream>

Server::Server(boost::asio::io_context* io_context, unsigned short port)
    : acceptor_(*io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    DoAccept();
}

void Server::DoAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<HTTPSession>(std::move(socket))->Run();
            }
            DoAccept();
        });
}
