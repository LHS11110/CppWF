#pragma once

#include "cppwf/ssl/ssl_context.hpp"
#include "cppwf/ssl/ssl_connection.hpp"
#include "cppwf/router/router.hpp"

#include <string>

namespace cppwf {
namespace server {

/**
 * @brief HTTPS 서버
 *
 * 흐름:
 *   TCP Accept → SslConnection(핸드셰이크) → HTTP 파싱
 *   → 라우터 디스패치 → 응답 전송
 */
class Server {
public:
    Server();
    ~Server();

    // 인증서 / 개인키 로드 (PEM 형식)
    bool load_certificate(const std::string& cert_path);
    bool load_private_key(const std::string& key_path);

    // 라우터 참조 반환
    router::Router& router() { return router_; }

    // 서버 시작 — port 기본값 443
    void listen(int port = 443);

private:
    // 클라이언트 연결 1개를 처리
    void handle_client(int client_fd);

    // SslConnection 스트림에서 HTTP 요청 파싱
    http::Request parse_request(ssl::SslConnection& conn);

    // HTTP 응답을 SslConnection 스트림으로 전송
    void send_response(ssl::SslConnection& conn, const http::Response& res);

    int             server_fd_ = -1;
    ssl::SslContext ssl_ctx_;
    router::Router  router_;
};

} // namespace server
} // namespace cppwf
