#pragma once

#include "crypto/bytes.hpp"

#include <openssl/ripemd.h>

namespace crypto {

auto ripemd160(const std::string &data) -> std::string {
    auto bytes = std::vector<std::uint8_t>(RIPEMD160_DIGEST_LENGTH);

    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);
    RIPEMD160_Update(&ctx, data.c_str(), data.length());
    RIPEMD160_Final(bytes.data(), &ctx);

    return bytes_to_hex(bytes);
}

}
