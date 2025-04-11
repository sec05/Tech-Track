#include "HttpPoller.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

HttpPoller& HttpPoller::instance(const std::vector<std::string>& urls, int interval_sec) {
    static HttpPoller instance(urls, interval_sec);
    return instance;
}

HttpPoller::HttpPoller(const std::vector<std::string>& urls, int interval_sec)
    : endpoints(urls), interval(interval_sec), timer(io_context), work_guard(net::make_work_guard(io_context)) {
    io_thread = std::thread([this]() { io_context.run(); });
}

void HttpPoller::start() {
    schedule_poll();
}

void HttpPoller::stop() {
    io_context.stop();
    if (io_thread.joinable()) io_thread.join();
}

std::unordered_map<std::string, std::string> HttpPoller::getResponsesCopy() {
    std::lock_guard<std::mutex> lock(data_mutex);
    return responses;
}

void HttpPoller::withResponses(const std::function<void(const std::unordered_map<std::string, std::string>&)>& visitor) {
    std::lock_guard<std::mutex> lock(data_mutex);
    visitor(responses);
}

void HttpPoller::schedule_poll() {
    timer.expires_after(std::chrono::seconds(interval));
    timer.async_wait([this](boost::system::error_code ec) {
        if (!ec) {
            poll_all();
            schedule_poll();
        }
    });
}

void HttpPoller::poll_all() {
    for (const auto& url : endpoints) {
        std::thread([this, url]() {
            try {
                std::string host, port = "80", target;

                auto protocol_pos = url.find("//");
                auto host_start = (protocol_pos != std::string::npos) ? protocol_pos + 2 : 0;
                auto path_start = url.find('/', host_start);

                host = url.substr(host_start, path_start - host_start);
                target = (path_start != std::string::npos) ? url.substr(path_start) : "/";

                net::io_context ioc;
                tcp::resolver resolver(ioc);
                beast::tcp_stream stream(ioc);

                auto const results = resolver.resolve(host, port);
                stream.connect(results);

                http::request<http::string_body> req{http::verb::get, target, 11};
                req.set(http::field::host, host);
                req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

                http::write(stream, req);

                beast::flat_buffer buffer;
                http::response<http::string_body> res;
                http::read(stream, buffer, res);

                stream.socket().shutdown(tcp::socket::shutdown_both);

                {
                    std::lock_guard<std::mutex> lock(data_mutex);
                    responses[url] = res.body();
                }

            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Failed to GET " << url << ": " << e.what() << std::endl;
            }
        }).detach();
    }
}
