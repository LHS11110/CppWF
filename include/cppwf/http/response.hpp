#pragma once

#include <string>
#include <unordered_map>

namespace cppwf {
namespace http {

class Response {
public:
    int status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    void set_body(const std::string& content, const std::string& content_type = "text/plain");
};

} // namespace http
} // namespace cppwf
