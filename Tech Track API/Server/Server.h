// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_SERVER_SERVER_H_
#define TECH_TRACK_API_SERVER_SERVER_H_

#include "HTTPSession.h"

#include <string>
#include <vector>

#include <boost/asio.hpp>
class Server {
 public:
    Server(boost::asio::io_context* io_context, unsigned short port, std::string config_file);
 private:
    void DoAccept();
    void ReadConfig(std::string filename);
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<std::string> valid_technologies_;
    std::vector<std::string> valid_companies_;
};

#endif  // TECH_TRACK_API_SERVER_SERVER_H_
