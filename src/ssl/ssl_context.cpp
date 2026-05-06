#include "cppwf/ssl/ssl_context.hpp"
#include <openssl/err.h>
#include <stdexcept>

namespace cppwf {
namespace ssl {

SslContext::SslContext() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx_ = SSL_CTX_new(TLS_server_method());
    if (!ctx_) {
        throw std::runtime_error("Failed to create SSL context");
    }
}

SslContext::~SslContext() {
    if (ctx_) {
        SSL_CTX_free(ctx_);
    }
}

bool SslContext::load_certificate(const std::string& cert_path) {
    return SSL_CTX_use_certificate_file(ctx_, cert_path.c_str(), SSL_FILETYPE_PEM) == 1;
}

bool SslContext::load_private_key(const std::string& key_path) {
    return SSL_CTX_use_PrivateKey_file(ctx_, key_path.c_str(), SSL_FILETYPE_PEM) == 1;
}

} // namespace ssl
} // namespace cppwf
