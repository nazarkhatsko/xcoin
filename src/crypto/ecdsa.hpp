#pragma once

#include "crypto/sha.hpp"
#include "crypto/bytes.hpp"

#include <secp256k1.h>

namespace crypto {

class ecdsa {
public:
    explicit ecdsa() {
        _ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    }

    virtual ~ecdsa() {
        secp256k1_context_destroy(_ctx);
    }

    auto private_key() -> std::vector<std::uint8_t> const {
        auto randomize = generate_random_bytes(32);
        if (not secp256k1_context_randomize(_ctx, randomize.data())) {
            throw std::logic_error("failed to generate randomness");
        }

        auto seckey = std::vector<std::uint8_t>(32);
        while (true) {
            seckey = generate_random_bytes(seckey.size());
            if (secp256k1_ec_seckey_verify(_ctx, seckey.data())) {
                break;
            }
        }

        return seckey;
    }

    auto public_key(const std::vector<std::uint8_t> &seckey) -> std::vector<std::uint8_t> const {
        if (not secp256k1_ec_pubkey_create(_ctx, &_pubkey, seckey.data())) {
            throw std::logic_error("unknown error");
        }

        auto compressed_pubkey = std::vector<std::uint8_t>(33);
        auto size = compressed_pubkey.size();
        if (not secp256k1_ec_pubkey_serialize(_ctx, compressed_pubkey.data(), &size, &_pubkey, SECP256K1_EC_COMPRESSED)) {
            throw std::logic_error("unknown error");
        }

        if (size != compressed_pubkey.size()) {
            throw std::logic_error("unknown error");
        }

        return compressed_pubkey;
    }

    auto signing(const std::vector<std::uint8_t> &seckey, const std::string &data) -> std::vector<std::uint8_t> const {
        auto bytes = hex_to_bytes(sha256(data));
        if (not secp256k1_ecdsa_sign(_ctx, &_signature, bytes.data(), seckey.data(), nullptr, nullptr)) {
            throw std::logic_error("unknown error");
        }

        auto serialized_signature = std::vector<std::uint8_t>(64);
        if (not secp256k1_ecdsa_signature_serialize_compact(_ctx, serialized_signature.data(), &_signature)) {
            throw std::logic_error("unknown error");
        }

        return serialized_signature;
    }

    auto verification(const std::vector<std::uint8_t> &compressed_pubkey, const std::vector<std::uint8_t> &serialized_signature, const std::string &data) -> bool const {
        if (not secp256k1_ecdsa_signature_parse_compact(_ctx, &_signature, serialized_signature.data())) {
            throw std::logic_error("failed parsing the signature");
        }

        if (not secp256k1_ec_pubkey_parse(_ctx, &_pubkey, compressed_pubkey.data(), compressed_pubkey.size())) {
            throw std::logic_error("failed parsing the public key");
        }

        auto hex = hex_to_bytes(sha256(data));
        return secp256k1_ecdsa_verify(_ctx, &_signature, hex.data(), &_pubkey);
    }

private:
    secp256k1_context         *_ctx;
    secp256k1_pubkey           _pubkey;
    secp256k1_ecdsa_signature  _signature;
};

}
