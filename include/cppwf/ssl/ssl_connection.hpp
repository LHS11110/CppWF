#pragma once

#include "cppwf/ssl/ssl_context.hpp"

#include <openssl/ssl.h>

namespace cppwf {
namespace ssl {

/**
 * @brief 클라이언트와의 단일 TLS 연결을 관리하는 클래스
 *
 * SslContext로부터 SSL 객체를 생성하고, TLS 핸드셰이크 / 읽기 /
 * 쓰기 / 종료를 책임집니다.
 * 소멸 시 SSL 객체와 파일 디스크립터를 자동으로 해제합니다.
 */
class SslConnection {
public:
  /**
   * @param ctx       서버 SslContext — SSL_CTX* 참조
   * @param client_fd accept()로 얻은 클라이언트 소켓 fd
   */
  SslConnection(SslContext &ctx, int client_fd);
  ~SslConnection();

  // 복사 금지, 이동만 허용
  SslConnection(const SslConnection &) = delete;
  SslConnection &operator=(const SslConnection &) = delete;
  SslConnection(SslConnection &&) = default;

  /**
   * @brief TLS 핸드셰이크 수행 (서버 측 SSL_accept)
   * @return 성공 시 true, 실패 시 false (오류는 stderr 출력)
   */
  bool accept();

  /**
   * @brief TLS 스트림에서 데이터 읽기
   * @param buf  읽어들일 버퍼
   * @param len  최대 읽기 바이트 수
   * @return 실제 읽은 바이트 수, 오류/종료 시 ≤ 0
   */
  int read(char *buf, int len) const;

  /**
   * @brief TLS 스트림으로 데이터 쓰기
   * @param data 전송할 문자열
   * @return 실제 전송한 바이트 수, 오류 시 ≤ 0
   */
  int write(const std::string &data) const;

  /**
   * @brief 현재 협상된 TLS 버전 문자열 반환 (예: "TLSv1.3")
   */
  std::string version() const;

private:
  SSL *ssl_ = nullptr;
  int fd_ = -1;
};

} // namespace ssl
} // namespace cppwf
