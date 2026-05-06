#include "cppwf/server/server.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <string_view>
#include <stdexcept>
#include <cstring>

namespace cppwf {
namespace server {

// ─────────────────────────────────────────────
// 생성자 / 소멸자
// ─────────────────────────────────────────────

Server::Server() = default;

Server::~Server() {
    if (server_fd_ >= 0) {
        ::close(server_fd_);
    }
}

// ─────────────────────────────────────────────
// 인증서 / 개인키
// ─────────────────────────────────────────────

bool Server::load_certificate(const std::string& cert_path) {
    return ssl_ctx_.load_certificate(cert_path);
}

bool Server::load_private_key(const std::string& key_path) {
    return ssl_ctx_.load_private_key(key_path);
}

// ─────────────────────────────────────────────
// 서버 시작 — 소켓 생성 → 바인드 → 리슨 → Accept 루프
// ─────────────────────────────────────────────

void Server::listen(int port) {
    // 1. TCP 소켓 생성
    server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        throw std::runtime_error("socket() 실패: " + std::string(std::strerror(errno)));
    }

    // 2. SO_REUSEADDR — 서버 재시작 시 주소 재사용 허용
    int opt = 1;
    if (::setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsockopt() 실패: " + std::string(std::strerror(errno)));
    }

    // 3. 포트 바인드
    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(static_cast<uint16_t>(port));

    if (::bind(server_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        throw std::runtime_error("bind() 실패 (port=" + std::to_string(port) +
                                 "): " + std::strerror(errno));
    }

    // 4. 리슨 — 최대 대기 큐 128
    if (::listen(server_fd_, 128) < 0) {
        throw std::runtime_error("listen() 실패: " + std::string(std::strerror(errno)));
    }

    log("[CppWF] HTTPS 서버 시작 — 포트 " + std::to_string(port) + " 대기 중");

    // 5. Accept 루프
    while (true) {
        sockaddr_in client_addr{};
        socklen_t   client_len = sizeof(client_addr);

        int client_fd = ::accept(server_fd_,
                                 reinterpret_cast<sockaddr*>(&client_addr),
                                 &client_len);
        if (client_fd < 0) {
            log("[경고] accept() 실패: " + std::string(std::strerror(errno)));
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        ::inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        log("[연결] 클라이언트 접속: " + std::string(client_ip) +
            ":" + std::to_string(ntohs(client_addr.sin_port)));

        handle_client(client_fd);
    }
}

// ─────────────────────────────────────────────
// 클라이언트 연결 처리 — SslConnection → 파싱 → 라우터 → 응답
// ─────────────────────────────────────────────

void Server::handle_client(int client_fd) {
    ssl::SslConnection conn(ssl_ctx_, client_fd);

    // TLS 핸드셰이크
    if (!conn.accept()) {
        log("[오류] SSL 핸드셰이크 실패");
        return;
    }

    log("[TLS] 핸드셰이크 완료 — " + conn.version());

    // HTTP 요청 파싱
    http::Request req = parse_request(conn);

    // 응답 객체 초기화
    http::Response res;
    res.status_code    = 200;
    res.status_message = "OK";

    // 라우터 디스패치
    if (!router_.dispatch(req, res)) {
        res.status_code    = 404;
        res.status_message = "Not Found";
        res.set_body("404 Not Found", "text/plain");
    }

    // 응답 전송
    send_response(conn, res);

    log("[응답] " + std::to_string(res.status_code) +
        " " + req.method + " " + req.path);

    // conn 소멸 시 SSL_shutdown / SSL_free / close(fd) 자동 처리
}

// ─────────────────────────────────────────────
// HTTP 요청 파싱
// ─────────────────────────────────────────────

http::Request Server::parse_request(ssl::SslConnection& conn) {
    constexpr std::size_t BUF_SIZE = 8192;
    char buf[BUF_SIZE];
    std::memset(buf, 0, sizeof(buf));

    int n = conn.read(buf, BUF_SIZE - 1);
    if (n <= 0) return {};

    std::string_view raw_data(buf, n);
    http::Request req;
    std::size_t pos = 0;

    auto next_line = [&raw_data, &pos]() -> std::string_view {
        if (pos >= raw_data.size()) return {};
        auto end = raw_data.find('\n', pos);
        std::string_view res;
        if (end == std::string_view::npos) {
            res = raw_data.substr(pos);
            pos = raw_data.size();
        } else {
            res = raw_data.substr(pos, end - pos);
            pos = end + 1;
        }
        if (!res.empty() && res.back() == '\r') {
            res.remove_suffix(1);
        }
        return res;
    };

    // 요청 라인: METHOD PATH VERSION
    std::string_view line = next_line();
    if (!line.empty()) {
        auto space1 = line.find(' ');
        if (space1 != std::string_view::npos) {
            req.method = std::string(line.substr(0, space1));
            auto space2 = line.find(' ', space1 + 1);
            if (space2 != std::string_view::npos) {
                req.path = std::string(line.substr(space1 + 1, space2 - space1 - 1));
                req.version = std::string(line.substr(space2 + 1));
            } else {
                req.path = std::string(line.substr(space1 + 1));
            }
        }
    }

    // 헤더 파싱
    while (true) {
        line = next_line();
        if (line.empty()) break;

        auto colon = line.find(':');
        if (colon != std::string_view::npos) {
            std::string_view key = line.substr(0, colon);
            std::string_view val = line.substr(colon + 1);
            while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) {
                val.remove_prefix(1);
            }
            req.headers[std::string(key)] = std::string(val);
        }
    }

    // 바디 파싱 (Content-Length 기반)
    auto it = req.headers.find("Content-Length");
    if (it != req.headers.end()) {
        std::size_t len = std::stoul(it->second);
        std::size_t available = raw_data.size() - pos;
        std::size_t read_len = (len < available) ? len : available;
        req.body = std::string(raw_data.substr(pos, read_len));
        if (len > read_len) {
            req.body.resize(len, '\0');
        }
    }

    return req;
}

// ─────────────────────────────────────────────
// HTTP 응답 전송
// ─────────────────────────────────────────────

void Server::send_response(ssl::SslConnection& conn, const http::Response& res) {
    std::string response;
    
    // 예상 크기 계산하여 한 번만 할당 (오버헤드 방지)
    std::size_t reserve_size = 32 + res.status_message.size() + res.body.size();
    for (const auto& [key, val] : res.headers) {
        reserve_size += key.size() + val.size() + 4; // ": \r\n"
    }
    response.reserve(reserve_size);

    response.append("HTTP/1.1 ").append(std::to_string(res.status_code)).append(" ").append(res.status_message).append("\r\n");
    for (const auto& [key, val] : res.headers) {
        response.append(key).append(": ").append(val).append("\r\n");
    }
    response.append("\r\n").append(res.body);

    conn.write(response);
}

} // namespace server
} // namespace cppwf
