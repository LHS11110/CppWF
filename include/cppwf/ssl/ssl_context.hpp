#pragma once

#include <string>
#include <openssl/ssl.h>

namespace cppwf {
namespace ssl {

class SslContext {
public:
    SslContext();
    ~SslContext();

    bool load_certificate(const std::string& cert_path);
    bool load_private_key(const std::string& key_path);

    SSL_CTX* native_handle() const { return ctx_; }

private:
    SSL_CTX* ctx_ = nullptr;
};

} // namespace ssl
} // namespace cppwf
