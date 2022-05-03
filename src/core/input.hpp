#pragma once

#include "core/coin.hpp"
#include "crypto/sha.hpp"

#include <json.hpp>
using namespace nlohmann;

#include <string>
#include <sstream>

namespace core {

class input {
public:
    explicit input(
        const std::size_t &number,
        const std::string &address,
        const coin        &coins,
        const std::string &signature,
        const std::string &txid
    ):
        _number   (number),
        _address  (address),
        _coins    (coins),
        _signature(signature),
        _txid     (txid)
    {}

    explicit input(const json &j):
        _number   (j["number"]),
        _address  (j["address"]),
        _coins    (j["coins"]),
        _signature(j["signature"]),
        _txid     (j["txid"])
    {}

public:
    inline std::size_t get_number()    const noexcept { return _number; }
    inline std::string get_address()   const noexcept { return _address; }
    inline coin        get_coins()     const noexcept { return _coins; }
    inline std::string get_signature() const noexcept { return _signature; }
    inline std::string get_txid()      const noexcept { return _txid; }

public:
    bool operator==(const input &i) const noexcept { return hash() == i.hash(); }
    bool operator==(const json &j)  const noexcept { return to_json() == j; }

    bool operator!=(const input &i) const noexcept { return not (*this == i); }
    bool operator!=(const json &j)  const noexcept { return not (*this == j); }

    input & operator=(const input &i) noexcept { return (*this = i); }
    input & operator=(const json &j)  noexcept { return (*this = input(j)); }

    operator json() const noexcept { return to_json(); }

public:
    std::size_t size() const {
        return sizeof(*this);
    }

    std::string hash() const {
        return crypto::sha256(to_string());
    }

public:
    std::string to_string() const {
        auto ss = std::stringstream();
        ss << _number << _address << _coins << _signature << _txid;
        return ss.str();
    }

    json to_json() const {
        auto j = json({
            {"number",    _number},
            {"address",   _address},
            {"coins",     _coins},
            {"signature", _signature},
            {"txid",      _txid}
        });
        return j;
    }

public:
    friend std::ostream & operator<<(std::ostream &os, const input &i);

private:
    std::size_t _number;
    std::string _address;
    coin        _coins;
    std::string _signature;
    std::string _txid;
};

std::ostream & operator<<(std::ostream &os, const input &i){
    os << i.to_string();
    return os;
}

}
