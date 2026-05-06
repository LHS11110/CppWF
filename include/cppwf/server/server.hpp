#pragma once

#include "cppwf/ssl/ssl_context.hpp"
#include "cppwf/ssl/ssl_connection.hpp"
#include "cppwf/router/router.hpp"
#include "cppwf/server/logger.hpp"

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

    // 로그 파일 설정 (비어 있으면 터미널 출력, 값이 있으면 파일에 기록)
    void set_log_file(const std::string& filename) { log_file_ = filename; }

    // 현재 설정된 로그 파일 경로 반환
    const std::string& log_file() const { return log_file_; }

    // 라우터 참조 반환
    router::Router& router() { return router_; }

    // 서버 시작 — port 기본값 443
    void listen(int port = 443);

private:
    // log_file_을 자동으로 넘겨주는 내부 로그 헬퍼
    void log(const std::string& msg) const { cppwf::server::log(msg, log_file_); }

    // 클라이언트 연결 1개를 처리
    void handle_client(int client_fd);

    // SslConnection 스트림에서 HTTP 요청 파싱
    http::Request parse_request(ssl::SslConnection& conn);

    // HTTP 응답을 SslConnection 스트림으로 전송
    void send_response(ssl::SslConnection& conn, const http::Response& res);

    int             server_fd_ = -1;
    ssl::SslContext ssl_ctx_;
    router::Router  router_;
    std::string     log_file_;  // 비어 있으면 터미널, 값이 있으면 파일에 기록
};

} // namespace server
} // namespace cppwf
