#pragma once

#include <openssl/bn.h>

#include <string>
#include <vector>

namespace crypto {

auto base58_encode(const std::string &data) -> std::string {
    auto table = std::string(
        "123456789" \
        "ABCDEFGHJKLMNPQRSTUVWXYZ" \
        "abcdefghijkmnopqrstuvwxyz"
    );
    auto result = std::string();
    auto v      = std::vector<std::size_t>();

    BIGNUM *base58     = nullptr;
    BIGNUM *result_add = BN_new();
    BIGNUM *result_rem = BN_new();
    BN_CTX *bn_ctx     = BN_CTX_new();

    BN_dec2bn(&base58, "58");
    BN_hex2bn(&result_add, data.c_str());

    while (not BN_is_zero(result_add)) {
        BN_div(result_add, result_rem, result_add, base58, bn_ctx);
        v.push_back(std::stoll(BN_bn2dec(result_rem)));
    }

    for (int i = v.size() - 1; i > -1; i--) {
        result += table[v[i]];
    }

    BN_free(result_add);
    BN_free(result_rem);
    BN_CTX_free(bn_ctx);

    return result;
}

}
