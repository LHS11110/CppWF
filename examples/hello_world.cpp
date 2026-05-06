#include "cppwf/router/router.hpp"
#include "cppwf/http/request.hpp"
#include "cppwf/http/response.hpp"
#include <iostream>

int main() {
    cppwf::router::Router router;

    router.get("/", [](const cppwf::http::Request& req, cppwf::http::Response& res) {
        res.set_body("Hello, CppWF!", "text/plain");
    });

    // 라우터 테스트
    cppwf::http::Request req;
    req.method = "GET";
    req.path = "/";

    cppwf::http::Response res;
    if (router.dispatch(req, res)) {
        std::cout << "[200] " << res.body << std::endl;
    } else {
        std::cout << "[404] Not Found" << std::endl;
    }

    return 0;
}
