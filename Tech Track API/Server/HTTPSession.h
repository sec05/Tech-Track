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
#include <boost/beast/http/write.hpp>


namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

// HTTPSession handles an individual HTTP connection using Boost.Beast (plain HTTP version).
class HTTPSession : public std::enable_shared_from_this<HTTPSession> {
 public:
    using plain_stream = beast::tcp_stream;

    HTTPSession(plain_stream stream,
                std::vector<std::string> valid_technologies,
                std::vector<std::string> valid_companies);

    void Run();
    std::string GetCompany() const { return company_; }
    std::string GetTechnology() const { return technology_; }

 private:
    boost::beast::tcp_stream stream_;
    beast::flat_buffer buffer_;             // Buffer for reading
    http::request<http::string_body> req_;  // HTTP request container
    std::vector<std::string> valid_technologies_;
    std::vector<std::string> valid_companies_;
    std::string company_;
    std::string technology_;

    void DoRead();          // Read HTTP request
    void HandleRequest();   // Handle request → build response
    void SendBadRequest(const std::string& why);

    bool IsValidTechnology(std::string technology);
    bool IsValidCompany(std::string company);

    template <typename Response>
void DoWrite(Response&& res) {
    auto self = shared_from_this();
    auto sp = std::make_shared<typename std::decay<Response>::type>(std::forward<Response>(res));

    http::async_write(stream_, *sp,
        [this, self, sp](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
                return;
            }

            beast::error_code shutdown_ec;
            stream_.socket().shutdown(tcp::socket::shutdown_send, shutdown_ec);
        });
    }
};

#endif  // TECH_TRACK_API_SERVER_HTTPSESSION_H_
