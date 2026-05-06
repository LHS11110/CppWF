#include "cppwf/router/router.hpp"

namespace cppwf {
namespace router {

void Router::get(const std::string& path, Handler handler) {
    routes_["GET"][path] = std::move(handler);
}

void Router::post(const std::string& path, Handler handler) {
    routes_["POST"][path] = std::move(handler);
}

void Router::put(const std::string& path, Handler handler) {
    routes_["PUT"][path] = std::move(handler);
}

void Router::del(const std::string& path, Handler handler) {
    routes_["DELETE"][path] = std::move(handler);
}

bool Router::dispatch(const http::Request& req, http::Response& res) const {
    auto method_it = routes_.find(req.method);
    if (method_it == routes_.end()) return false;

    auto path_it = method_it->second.find(req.path);
    if (path_it == method_it->second.end()) return false;

    path_it->second(req, res);
    return true;
}

} // namespace router
} // namespace cppwf
