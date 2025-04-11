#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <thread>
#include <boost/asio.hpp>

class HttpPoller {
public:
    // Singleton accessor
    static HttpPoller& instance(const std::vector<std::string>& urls = {}, int interval_sec = 10);

    // Start/stop polling
    void start();
    void stop();

    // Safe access to response data
    std::unordered_map<std::string, std::string> getResponsesCopy();
    void withResponses(const std::function<void(const std::unordered_map<std::string, std::string>&)>& visitor);

private:
    // Private constructor for singleton
    HttpPoller(const std::vector<std::string>& urls, int interval_sec);

    // Non-copyable
    HttpPoller(const HttpPoller&) = delete;
    HttpPoller& operator=(const HttpPoller&) = delete;

    // Internals
    void schedule_poll();
    void poll_all();

    std::vector<std::string> endpoints;
    int interval;
    std::unordered_map<std::string, std::string> responses;
    std::mutex data_mutex;

    boost::asio::io_context io_context;
    boost::asio::steady_timer timer;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;
    std::thread io_thread;
};
