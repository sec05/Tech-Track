// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_SERVER_SERVER_H_
#define TECH_TRACK_API_SERVER_SERVER_H_

#include "HTTPSession.h"

#include <boost/asio.hpp>

class Server {
 public:
    Server(boost::asio::io_context* io_context, unsigned short port);

 private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif  // TECH_TRACK_API_SERVER_SERVER_H_
