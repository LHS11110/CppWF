#include "cppwf/http/response.hpp"

namespace cppwf {
namespace http {

void Response::set_body(const std::string& content, const std::string& content_type) {
    body = content;
    headers["Content-Type"] = content_type;
    headers["Content-Length"] = std::to_string(content.size());
}

} // namespace http
} // namespace cppwf
