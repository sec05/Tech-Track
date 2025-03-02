// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_SERVER_HTTPSESSION_H_
#define TECH_TRACK_API_SERVER_HTTPSESSION_H_

#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace asio = boost::asio;           // from <boost/asio.hpp>
using tcp = asio::ip::tcp;

// HTTPSession handles an individual HTTP connection using Boost.Beast.
class HTTPSession : public std::enable_shared_from_this<HTTPSession> {
 public:
    // Constructor: takes ownership of the socket.
    explicit HTTPSession(tcp::socket socket, std::vector<std::string> valid_technologies,
        std::vector<std::string> valid_companies);

    void Run();

 private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;             // Buffer for reading
    http::request<http::string_body> req_;  // Container for the HTTP request
    std::vector<std::string> valid_technologies_;
    std::vector<std::string> valid_companies_;

    // Asynchronously read an HTTP request.
    void DoRead();

    // Process the request and generate a response.
    void HandleRequest();

    // Helper to send a bad request response.
    void SendBadRequest(const std::string& why);

    bool IsValidTechnology(std::string technology);
    bool IsValidCompany(std::string company);

    // Asynchronously write the response.
    // This is a template so it can handle different response types.
    template<class Response>
    void DoWrite(Response&& res) {
        auto self = shared_from_this();
        // Keep the response alive until the async_write is complete.
        auto sp = std::make_shared<typename std::decay<Response>::type>
        (std::forward<Response>(res));
        http::async_write(socket_, *sp,
            [this, self, sp](beast::error_code ec, std::size_t bytes_transferred) {
                boost::ignore_unused(bytes_transferred);
                if (ec) {
                    std::cerr << "Write error: " << ec.message() << std::endl;
                }
                // Gracefully close the socket after sending the response.
                beast::error_code ec_shutdown;
                socket_.shutdown(tcp::socket::shutdown_send, ec_shutdown);
            });
    }
};

#endif  // TECH_TRACK_API_SERVER_HTTPSESSION_H_
