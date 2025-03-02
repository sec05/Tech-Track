// Copyright 2025 Spencer Evans-Cole
#include "Server.h"

#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>

Server::Server(boost::asio::io_context* io_context, unsigned short port, std::string config_file)
    : acceptor_(*io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    ReadConfig(config_file);
    DoAccept();
}

void Server::DoAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<HTTPSession>
                (std::move(socket), valid_technologies_, valid_companies_)->Run();
            }
            DoAccept();
        });
}

void Server::ReadConfig(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open config file: "<< filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("Technologies:") != std::string::npos) {
            line = line.substr(line.find("Technologies:")+13);
            while (line.find(",") != std::string::npos) {
                std::string value = line.substr(0, line.find(","));
                value = std::regex_replace(value, std::regex("^ +"), "");
                valid_technologies_.push_back(value);
                line = line.substr(line.find(",") + 1);
            }
            std::string value = line.substr(0, line.find(","));
            value = std::regex_replace(value, std::regex("^ +"), "");
            valid_technologies_.push_back(value);
        } else if (line.find("Companies:") != std::string::npos) {
            line = line.substr(line.find("Companies:")+10);
            while (line.find(",") != std::string::npos) {
                std::string value = line.substr(0, line.find(","));
                value = std::regex_replace(value, std::regex("^ +"), "");
                valid_companies_.push_back(value);
                line = line.substr(line.find(",") + 1);
            }
            std::string value = line.substr(0, line.find(","));
            value = std::regex_replace(value, std::regex("^ +"), "");
            valid_companies_.push_back(value);
        } else {
            std::cerr << "Invalid line in config file: " << line << std::endl;
        }
    }
    file.close();
}
