#pragma once

#include "cppwf/http/method.hpp"
#include <string>
#include <unordered_map>

namespace cppwf {
namespace http {

class Request {
public:
    HttpMethod method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

} // namespace http
} // namespace cppwf
