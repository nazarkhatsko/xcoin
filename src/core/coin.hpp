#pragma once

#include "defines.hpp"

#include <json.hpp>
using namespace nlohmann;

#include <cmath>
#include <string>
#include <ostream>

namespace core {

class coin {
public:
    explicit coin():
        coin(COIN_PRECISION, COIN_SYMBOL, 0, COIN_NAME)
    {}

    explicit coin(std::size_t amount):
        coin(COIN_PRECISION, COIN_SYMBOL, amount, COIN_NAME)
    {}

    explicit coin(
        const std::size_t &precision,
        const std::string &symbol,
        const std::size_t &amount,
        const std::string &name
    ):
        _precision(precision),
        _symbol   (symbol),
        _amount   (amount),
        _name     (name)
    {}

    explicit coin(const json &j):
        _precision(j["precision"]),
        _symbol   (j["symbol"]),
        _amount   (j["amount"]),
        _name     (j["name"])
    {}

public:
    inline std::size_t get_precision() const noexcept { return _precision; }
    inline std::string get_symbol()    const noexcept { return _symbol; }
    inline std::size_t get_amount()    const noexcept { return _amount; }
    inline std::string get_name()      const noexcept { return _name; }

public:
    bool operator==(const coin &c) const noexcept { return to_json() == c.to_json(); }
    bool operator==(const json &j) const noexcept { return to_json() == j; }

    bool operator!=(const coin &c) const noexcept { return not (*this == c); }
    bool operator!=(const json &j) const noexcept { return not (*this == j); }

    bool operator<(const coin &c) const noexcept { return _amount < c._amount; }
    bool operator<(const json &j) const noexcept { return (*this < coin(j)); }

    bool operator>(const coin &c) const noexcept { return _amount > c._amount; }
    bool operator>(const json &j) const noexcept { return (*this > coin(j)); }

    bool operator>=(const coin &c) const noexcept { return (*this > c or *this == c); }
    bool operator>=(const json &j) const noexcept { return (*this >= coin(j)); }

    bool operator<=(const coin &c) const noexcept { return (*this < c or *this == c); }
    bool operator<=(const json &j) const noexcept { return (*this <= coin(j)); }

    coin & operator=(const coin &c) noexcept { return (*this = c); }
    coin & operator=(const json &j) noexcept { return (*this = coin(j)); }

    coin operator+(const coin &c) const noexcept {
        auto tmp = *this;
        tmp._amount += c._amount;
        return tmp;
    }

    coin operator-(const coin &c) const noexcept {
        auto tmp = *this;
        tmp._amount -= c._amount;
        return tmp;
    }

    coin & operator+=(const coin &c) noexcept { return (*this = *this + c); }
    coin & operator+=(const json &j) noexcept { return (*this = *this + coin(j)); }

    coin & operator-=(const coin &c) noexcept { return (*this = *this - c); }
    coin & operator-=(const json &j) noexcept { return (*this = *this - coin(j)); }

    operator json()   const noexcept { return to_json(); }
    operator double() const noexcept { return to_double(); }

public:
    std::string to_string() const {
        // auto amount_str = std::to_string(_amount);
        // amount_str.insert(amount_str.length() - _precision, ".");
        // return amount_str;
        return std::to_string(to_double());
    }

    json to_json() const {
        auto j = json({
            {"precision", _precision},
            {"symbol",    _symbol},
            {"amount",    _amount},
            {"name",      _name},
        });
        return j;
    }

    double to_double() const {
        if (_amount == 0) {
            return 0;
        }
        return static_cast<double>(_amount) / std::pow(10, static_cast<double>(_precision));
    }

public:
    friend std::ostream & operator<<(std::ostream &os, const coin &c);

private:
    std::size_t _precision;
    std::string _symbol;
    std::size_t _amount;
    std::string _name;
};

std::ostream & operator<<(std::ostream &os, const coin &c){
    os << c.to_string();
    return os;
}

}
