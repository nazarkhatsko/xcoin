#pragma once

#include "crypto/bytes.hpp"

#include <openssl/sha.h>

namespace crypto {

auto sha256(const std::string &data) -> std::string {
    auto bytes = std::vector<std::uint8_t>(SHA256_DIGEST_LENGTH);

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, data.c_str(), data.length());
    SHA256_Final(bytes.data(), &ctx);

    return bytes_to_hex(bytes);
}

}