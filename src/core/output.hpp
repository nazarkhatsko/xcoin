#pragma once

#include "core/coin.hpp"
#include "crypto/sha.hpp"

#include <json.hpp>
using namespace nlohmann;

#include <string>
#include <sstream>

namespace core {

class output {
public:
    explicit output(
        const std::size_t &number,
        const std::string &address,
        const coin        &coins,
        const std::string &public_key,
        const std::string &txid
    ):
        _number    (number),
        _address   (address),
        _coins     (coins),
        _public_key(public_key),
        _txid      (txid)
    {}

    explicit output(const json &j):
        _number    (j["number"]),
        _address   (j["address"]),
        _coins     (j["coins"]),
        _public_key(j["public_key"]),
        _txid      (j["txid"])
    {}

public:
    inline std::size_t get_number()     const noexcept { return _number; }
    inline std::string get_address()    const noexcept { return _address; }
    inline coin        get_coins()      const noexcept { return _coins; }
    inline std::string get_public_key() const noexcept { return _public_key; }
    inline std::string get_txid()       const noexcept { return _txid; }

public:
    bool operator==(const output &o) const noexcept { return hash() == o.hash(); }
    bool operator==(const json &j)   const noexcept { return to_json() == j; }

    bool operator!=(const output &o) const noexcept { return not (*this == o); }
    bool operator!=(const json &j)   const noexcept { return not (*this == j); }

    output & operator=(const output &o) noexcept { return (*this = o); }
    output & operator=(const json &j)   noexcept { return (*this = output(j)); }

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
        ss << _number << _address << _coins << _public_key << _txid;
        return ss.str();
    }

    json to_json() const {
        auto j = json({
            {"number",     _number},
            {"address",    _address},
            {"coins",      _coins},
            {"public_key", _public_key},
            {"txid",       _txid}
        });
        return j;
    }

public:
    friend std::ostream & operator<<(std::ostream &os, const output &o);

private:
    std::size_t _number;
    std::string _address;
    coin        _coins;
    std::string _public_key;
    std::string _txid;
};

std::ostream & operator<<(std::ostream &os, const output &o){
    os << o.to_string();
    return os;
}

}
