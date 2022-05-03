#pragma once

#include "defines.hpp"
#include "core/transaction.hpp"
#include "crypto/merkletree.hpp"

#include <forward_list>

namespace core {

class block {
public:
    explicit block(
        const std::size_t                    &number,
        const std::string                    &prev_block,
        const std::forward_list<transaction> &transactions
    ):
        _number      (number),
        _prev_block  (prev_block),
        _version     (BLOCK_VERSION),
        _nonce       (0),
        _timestamp   (tools::clock_now()),
        _transactions(transactions)
    {}

    explicit block(const json &j):
        _number      (j["number"]),
        _prev_block  (j["previous_block"]),
        _version     (j["version"]),
        _nonce       (j["nonce"]),
        _timestamp   (tools::clock_now()),
        _transactions(std::accumulate(j["transactions"].begin(), j["transactions"].end(), std::forward_list<transaction>(), [](std::forward_list<transaction> l, json j) {
            l.push_front(transaction(j));
            return l;
        }))
    {}

public:
    inline std::size_t  get_number() const noexcept         { return _number; }
    inline std::string  get_previous_block() const noexcept { return _prev_block; }
    inline std::uint8_t get_version() const noexcept        { return _version; }
    inline std::size_t  get_nonce() const noexcept          { return _nonce; }
    inline std::time_t  get_timestamp() const noexcept      { return _timestamp; }

public:
    bool operator==(const block &b) const noexcept { return hash() == b.hash(); }
    bool operator==(const json &j)  const noexcept { return to_json() == j; }

    bool operator!=(const block &b) const noexcept { return not (*this == b); }
    bool operator!=(const json &j)  const noexcept { return not (*this == j); }

    block & operator=(const block &b) noexcept { return (*this = b); }
    block & operator=(const json &j)  noexcept { return (*this = block(j)); }

    operator json() const noexcept { return to_json(); }

    const transaction & operator[](std::size_t i) const noexcept {
        auto itr = _transactions.begin();
        std::advance(itr, i);
        return *itr;
    }

    block & operator++(int) noexcept {
        _nonce++;
        return *this;
    }

public:
    inline bool empty() const noexcept {
        return _transactions.empty();
    }

    // const transaction & top() const {
    //     return _transactions.front();
    // }

    void push(const transaction &t) noexcept {
        _transactions.push_front(t);
    }

    void clear() noexcept {
        _transactions.clear();
    }

    const core::transaction * find(const std::string &hash) const noexcept {
        const auto itr = std::find_if(_transactions.begin(), _transactions.end(), [&](const core::transaction &t) {
            return t.hash() == hash;
        });
        if (itr != _transactions.end()) {
            return &(*itr);
        }
        return nullptr;
    }

    std::forward_list<transaction>::iterator begin() noexcept { return _transactions.begin(); }
    std::forward_list<transaction>::iterator end()   noexcept { return _transactions.end(); }

    std::forward_list<transaction>::const_iterator begin() const noexcept { return _transactions.begin(); }
    std::forward_list<transaction>::const_iterator end()   const noexcept { return _transactions.end(); }

public:
    std::size_t size() const {
        return sizeof(*this);
    }

    std::size_t length() const {
        return std::distance(_transactions.begin(), _transactions.end());
    }

    std::string merkle_root() const {
        // auto v = std::vector<std::string>();
        // for (const auto &i : _transactions) {
        //     v.push_back(i.to_string());
        // }
        // auto mt = crypto::merkletree(v);
        // return mt.root();
        return "none";
    }

    std::string hash() const {
        return crypto::sha256(to_string());
    }

public:
    std::string to_string() const {
        auto ss = std::stringstream();
        ss << _number << _prev_block << _nonce << _timestamp << merkle_root();// << _version;
        return ss.str();
    }

    json to_json() const {
        auto j = json({
            {"number",             _number},
            {"previous_block",     _prev_block},
            {"version",            _version},
            {"nonce",              _nonce},
            {"timestamp",          _timestamp},
            {"merkle_root",        merkle_root()},
            {"hash",               hash()},
            {"transactions_count", length()},
            {"transactions",       json::array()}
        });
        for (const auto &i : _transactions) {
            j["transactions"].push_back(i);
        }
        return j;
    }

public:
    friend std::ostream & operator<<(std::ostream &os, const block &b);

private:
    std::size_t                    _number;
    std::string                    _prev_block;
    std::uint8_t                   _version;
    std::size_t                    _nonce;
    std::time_t                    _timestamp;
    std::forward_list<transaction> _transactions;
};

std::ostream & operator<<(std::ostream &os, const block &b){
    os << b.to_string();
    return os;
}

}


 // bool is_done() const {
    //     return size() >= BLOCK_MAX_SIZE or _timestamp + BLOCK_MAX_TIME <= tools::clock_now();
    // }

    // bool is_valid() const {
    //     auto hash_str = hash();
    //     return not hash_str.empty() and hash_str.substr(0, MINE_DIFFICULTY) == std::string(MINE_DIFFICULTY, MINE_BITS);
    // }

    // void mine() {
    //     if (not is_done()) {
    //         throw std::runtime_error("my error 2");
    //     }
    //     do {
    //         _nonce++;
    //         hash();
    //     } while (not is_valid());
    // }

// public:
//     const transaction * find_by_hash(const hash_t &hash) const noexcept {
//         const auto itr = std::find_if(_transactions.begin(), _transactions.end(), [&](const auto &item) {
//             return item.hash() == hash;
//         });
//         if (itr != _transactions.end()) {
//             return &(*itr);
//         }
//         return nullptr;
//     }

//     const transaction * operator[](std::size_t i) const noexcept { // fix
//         return nullptr;
//     }
