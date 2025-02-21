// Copyright 2025 Spencer Evans-Cole

#include <iostream>

#include <boost/asio.hpp>

#include "Server/Server.h"

int main() {
    try {
        boost::asio::io_context* io_context =  new boost::asio::io_context;
        Server server(io_context, 8008);  // Listening on port 12345
        io_context->run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
