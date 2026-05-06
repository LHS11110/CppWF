#include "cppwf/server/server.hpp"
#include <iostream>

int main() {
    cppwf::server::Server app;

    // 인증서 및 개인키 로드
    if (!app.load_certificate("cert.pem")) {
        std::cerr << "[오류] 인증서 로드 실패: cert.pem\n";
        return 1;
    }
    if (!app.load_private_key("key.pem")) {
        std::cerr << "[오류] 개인키 로드 실패: key.pem\n";
        return 1;
    }

    // 라우트 등록
    app.router().get("/", [](const cppwf::http::Request&,
                              cppwf::http::Response& res) {
        res.set_body("<h1>Hello, CppWF!</h1>", "text/html");
    });

    app.router().get("/health", [](const cppwf::http::Request&,
                                    cppwf::http::Response& res) {
        res.set_body("{\"status\":\"ok\"}", "application/json");
    });

    // 443 포트에서 HTTPS 서버 시작
    app.listen(443);

    return 0;
}
