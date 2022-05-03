#pragma once

#include "core/coin.hpp"
#include "core/input.hpp"
#include "core/output.hpp"
#include "tools/clock.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <algorithm>

namespace core {

class transaction {
public:
    explicit transaction(
        const std::string         &memo,
        const coin                &fee,
        const std::vector<input>  &inputs,
        const std::vector<output> &outputs
    ):
        _memo  (memo),
        _fee   (fee),
        inputs (inputs),
        outputs(outputs)
    {}

    explicit transaction(const json &j):
        _memo     (j["memo"]),
        _fee      (j["fee"]),
        _timestamp(j["timestamp"]),
        inputs(std::accumulate(j["inputs"].begin(), j["inputs"].end(), std::vector<input>(), [](std::vector<input> v, json j) {
            v.push_back(input(j));
            return v;
        })),
        outputs(std::accumulate(j["outputs"].begin(), j["outputs"].end(), std::vector<output>(), [](std::vector<output> v, json j) {
            v.push_back(output(j));
            return v;
        }))
    {}

public:
    inline std::string get_memo()      const noexcept { return _memo; }
    inline coin        get_fee()       const noexcept { return _fee; }
    inline std::time_t get_timestamp() const noexcept { return _timestamp; }

public:
    bool operator==(const transaction &t) const noexcept { return hash() == t.hash(); }
    bool operator==(const json &j)        const noexcept { return to_json() == j; }

    bool operator!=(const transaction &t) const noexcept { return not (*this == t); }
    bool operator!=(const json &j)        const noexcept { return not (*this == j); }

    transaction & operator=(const transaction &t) noexcept { return (*this = t); }
    transaction & operator=(const json &j)        noexcept { return (*this = transaction(j)); }

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
        ss << _memo << _fee << _timestamp;
        for (const auto &i : inputs) {
            ss << i;
        }
        for (const auto &i : outputs) {
            ss << i;
        }
        return ss.str();
    }

    json to_json() const {
        auto j = json({
            {"memo",      _memo},
            {"fee",       _fee},
            {"timestamp", _timestamp},
            {"hash",      hash()},
            {"inputs",    json::array()},
            {"outputs",   json::array()}
        });
        for (const auto &i : inputs) {
            j["inputs"].push_back(i);
        }
        for (const auto &i : outputs) {
            j["outputs"].push_back(i);
        }
        return j;
    }

public:
    friend std::ostream & operator<<(std::ostream &os, const transaction &t);

public:
    const std::vector<input>  inputs;
    const std::vector<output> outputs;

private:
    std::string _memo;
    coin        _fee;
    std::time_t _timestamp;
};

std::ostream & operator<<(std::ostream &os, const transaction &t){
    os << t.to_string();
    return os;
}

}
