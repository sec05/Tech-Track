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
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace asio = boost::asio;           // from <boost/asio.hpp>
using tcp = asio::ip::tcp;

// HTTPSession handles an individual HTTP connection using Boost.Beast.
class HTTPSession : public std::enable_shared_from_this<HTTPSession> {
 public:
    // Constructor: takes ownership of the socket.
    explicit HTTPSession(boost::asio::ssl::stream<boost::beast::tcp_stream>, std::vector<std::string> valid_technologies,
        std::vector<std::string> valid_companies);

    void Run();
    std::string GetCompany() const { return company_; }
    std::string GetTechnology() const { return technology_; }

 private:
    boost::asio::ssl::stream<boost::beast::tcp_stream> stream_;  // SSL stream for the connection
    beast::flat_buffer buffer_;             // Buffer for reading
    http::request<http::string_body> req_;  // Container for the HTTP request
    std::vector<std::string> valid_technologies_;
    std::vector<std::string> valid_companies_;
    std::string company_;
    std::string technology_;  // Company and technology from the request target
    // Asynchronously read an HTTP request.
    void DoRead();
    
    // Perform the SSL handshake.
    void DoHandshake();
    
    // Process the request and generate a response.
    void HandleRequest();

    // Helper to send a bad request response.
    void SendBadRequest(const std::string& why);

    bool IsValidTechnology(std::string technology);
    bool IsValidCompany(std::string company);

    // Asynchronously write the response.
    // This is a template so it can handle different response types.
    template <typename Response> void DoWrite(Response&& res) {
    auto self = shared_from_this();
    // Keep the response alive until async_write is complete.
    auto sp = std::make_shared<typename std::decay<Response>::type>(std::forward<Response>(res));

    http::async_write(stream_, *sp,
        [this, self, sp](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
                return;
            }

            // Gracefully shut down the SSL stream
            stream_.async_shutdown(
                [this, self](beast::error_code ec_shutdown) {
                    if (ec_shutdown) {
                        std::cerr << "SSL shutdown error: " << ec_shutdown.message() << std::endl;
                    }
                });
        });
}

};

#endif  // TECH_TRACK_API_SERVER_HTTPSESSION_H_
