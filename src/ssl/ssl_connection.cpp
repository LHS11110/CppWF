#include "cppwf/ssl/ssl_connection.hpp"

#include <openssl/err.h>
#include <unistd.h>

namespace cppwf {
namespace ssl {

// ─────────────────────────────────────────────
// 생성자 — SSL 객체 생성 및 fd 연결
// ─────────────────────────────────────────────

SslConnection::SslConnection(SslContext &ctx, int client_fd) : fd_(client_fd) {
  ssl_ = SSL_new(ctx.native_handle());
  if (!ssl_) {
    std::cerr << "[SslConnection] SSL_new() 실패\n";
    return;
  }
  SSL_set_fd(ssl_, fd_);
}

// ─────────────────────────────────────────────
// 소멸자 — SSL 종료 및 fd 닫기
// ─────────────────────────────────────────────

SslConnection::~SslConnection() {
  if (ssl_) {
    SSL_shutdown(ssl_);
    SSL_free(ssl_);
    ssl_ = nullptr;
  }
  if (fd_ >= 0) {
    ::close(fd_);
    fd_ = -1;
  }
}

// ─────────────────────────────────────────────
// TLS 핸드셰이크 (서버 측)
// ─────────────────────────────────────────────

bool SslConnection::accept() {
  if (!ssl_)
    return false;

  if (SSL_accept(ssl_) <= 0) {
    ERR_print_errors_fp(stderr);
    return false;
  }
  return true;
}

// ─────────────────────────────────────────────
// 읽기
// ─────────────────────────────────────────────

int SslConnection::read(char *buf, int len) const {
  if (!ssl_)
    return -1;
  return SSL_read(ssl_, buf, len);
}

// ─────────────────────────────────────────────
// 쓰기
// ─────────────────────────────────────────────

int SslConnection::write(const std::string &data) const {
  if (!ssl_)
    return -1;
  return SSL_write(ssl_, data.c_str(), static_cast<int>(data.size()));
}

// ─────────────────────────────────────────────
// TLS 버전 문자열
// ─────────────────────────────────────────────

std::string SslConnection::version() const {
  if (!ssl_)
    return "unknown";
  const char *ver = SSL_get_version(ssl_);
  return ver ? ver : "unknown";
}

} // namespace ssl
} // namespace cppwf
