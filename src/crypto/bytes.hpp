#pragma once

#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>

namespace crypto {

auto hex_to_bytes(const std::string &hex) -> std::vector<std::uint8_t> {
    auto bytes = std::vector<std::uint8_t>();
    for (const auto &i : hex) {
        bytes.push_back(static_cast<std::uint8_t>(i));
    }
    return bytes;
}

auto bytes_to_hex(const std::vector<std::uint8_t> &bytes) -> std::string {
    auto hex = std::stringstream();
    hex << std::hex << std::setfill('0');
    for (const auto &i : bytes) {
        hex << std::setw(2) << static_cast<std::uint32_t>(i);
    }
    return hex.str();
}

auto generate_random_bytes(const std::size_t &n) -> std::vector<std::uint8_t> {
    auto bytes = std::vector<std::uint8_t>(n);
    auto rd = std::random_device();
    auto mt = std::mt19937(rd());
    auto uid = std::uniform_real_distribution<double>(0.0, 255.0);
    for (auto &i : bytes) {
        i = uid(mt);
    }
    return bytes;

    // auto bytes = bytes_t(n);
    // auto dre = std::default_random_engine();
    // auto uid = std::uniform_int_distribution<std::uint8_t>(0, 255);
    // for (auto &i : bytes) {
    //     i = uid(dre);
    // }
    // return bytes;

    // auto bytes = bytes_t(n);
    // auto rd = std::random_device();
    // for (auto &i : bytes) {
    //     i = rd.entropy();
    // }
    // return bytes;
}

}
