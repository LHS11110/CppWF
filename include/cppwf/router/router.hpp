#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "cppwf/http/request.hpp"
#include "cppwf/http/response.hpp"

namespace cppwf {
namespace router {

using Handler = std::function<void(const http::Request&, http::Response&)>;

class Router {
public:
    void get(const std::string& path, Handler handler);
    void post(const std::string& path, Handler handler);
    void put(const std::string& path, Handler handler);
    void del(const std::string& path, Handler handler);

    bool dispatch(const http::Request& req, http::Response& res) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, Handler>> routes_;
};

} // namespace router
} // namespace cppwf
