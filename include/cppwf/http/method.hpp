#pragma once

#include <string>

#include <unordered_map>

namespace cppwf {
namespace http {

enum class HttpMethod {
    GET = 0,
    POST = 1,
    PUT = 2,
    DELETE = 3,
    UNKNOWN = 4
};

inline HttpMethod parse_method(const std::string& method_str) {
    static const std::unordered_map<std::string, HttpMethod> map = {
        {"GET", HttpMethod::GET},
        {"POST", HttpMethod::POST},
        {"PUT", HttpMethod::PUT},
        {"DELETE", HttpMethod::DELETE}
    };
    
    auto it = map.find(method_str);
    if (it != map.end()) {
        return it->second;
    }
    return HttpMethod::UNKNOWN;
}

inline std::string to_string(HttpMethod method) {
    switch (method) {
        case HttpMethod::GET: return "GET";
        case HttpMethod::POST: return "POST";
        case HttpMethod::PUT: return "PUT";
        case HttpMethod::DELETE: return "DELETE";
        default: return "UNKNOWN";
    }
}

} // namespace http
} // namespace cppwf
