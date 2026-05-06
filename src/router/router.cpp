#include "cppwf/router/router.hpp"

namespace cppwf {
namespace router {

void Router::get(const std::string& path, Handler handler) {
    routes_[static_cast<int>(http::HttpMethod::GET)][path] = handler;
}

void Router::post(const std::string& path, Handler handler) {
    routes_[static_cast<int>(http::HttpMethod::POST)][path] = handler;
}

void Router::put(const std::string& path, Handler handler) {
    routes_[static_cast<int>(http::HttpMethod::PUT)][path] = handler;
}

void Router::del(const std::string& path, Handler handler) {
    routes_[static_cast<int>(http::HttpMethod::DELETE)][path] = handler;
}

bool Router::dispatch(const http::Request& req, http::Response& res) const {
    if (req.method == http::HttpMethod::UNKNOWN) return false;
    
    int method_idx = static_cast<int>(req.method);
    auto path_it = routes_[method_idx].find(req.path);
    if (path_it == routes_[method_idx].end()) return false;

    path_it->second(req, res);
    return true;
}

} // namespace router
} // namespace cppwf
