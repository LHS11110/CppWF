#pragma once

#include <functional>
#include "cppwf/http/request.hpp"
#include "cppwf/http/response.hpp"

namespace cppwf {
namespace middleware {

using NextFn = std::function<void()>;
using MiddlewareFn = std::function<void(http::Request&, http::Response&, NextFn)>;

} // namespace middleware
} // namespace cppwf
