#pragma once

#include "defines.hpp"
#include "core/block.hpp"
#include "core/transaction.hpp"

namespace core {

class chain {
public:
    explicit chain() {}

    explicit chain(const std::forward_list<block> &blocks):
        _blocks(blocks)
    {
        _sort();
    }

    explicit chain(const json &j):
        _blocks(std::accumulate(j["blocks"].begin(), j["blocks"].end(), std::forward_list<block>(), [](std::forward_list<block> l, json j) {
            l.push_front(block(j));
            return l;
        }))
    {
        _sort();
    }

public:
    bool operator==(const chain &c) const noexcept { return to_json() == c.to_json(); }
    bool operator==(const json &j)  const noexcept { return to_json() == j; }

    bool operator!=(const chain &c) const noexcept { return not (*this == c); }
    bool operator!=(const json &j)  const noexcept { return not (*this == j); }

    chain & operator=(const chain &c) noexcept { return (*this = c); }
    chain & operator=(const json &j)  noexcept { return (*this = chain(j)); }

    operator json() const noexcept { return to_json(); }

    const block & operator[](std::size_t i) const noexcept {
        auto itr = _blocks.begin();
        std::advance(itr, i);
        return *itr;
    }

public:
    inline bool empty() const noexcept {
        return _blocks.empty();
    }

    const block & top() const {
        return _blocks.front();
    }

    void push(const block &b) noexcept {
        _blocks.push_front(b);
    }

    void clear() noexcept {
        _blocks.clear();
    }

    const core::block * find(const std::string &hash) const noexcept {
        const auto itr = std::find_if(_blocks.begin(), _blocks.end(), [&](const core::block &b) {
            return b.hash() == hash;
        });
        if (itr != _blocks.end()) {
            return &(*itr);
        }
        return nullptr;
    }

    std::forward_list<block>::iterator begin() noexcept { return _blocks.begin(); }
    std::forward_list<block>::iterator end() noexcept   { return _blocks.end(); }

    std::forward_list<block>::const_iterator begin() const noexcept { return _blocks.begin(); }
    std::forward_list<block>::const_iterator end() const noexcept   { return _blocks.end(); }

public:
    block create_new_block(const std::string &miner) const noexcept {
        if (empty()) {
            return block(0, "genesis", {_miner_reward(miner)});
        }

        return block(_blocks.front().get_number() + 1, _blocks.front().hash(), {_miner_reward(miner)});
    }

public:
    std::size_t size() const {
        return sizeof (*this) + sizeof (block) * length();
    }

    std::size_t length() const {
        return std::distance(_blocks.begin(), _blocks.end());
    }

public:
    json to_json() const {
        auto j = json::array();
        for (const auto &i : _blocks) {
            j.push_back(i);
        }
        return j;
    }

private:
    transaction _miner_reward(const std::string &address) const noexcept {
        const auto memo = "coinbase";
        const auto outputs = {
            output(0, address, coin(MINE_REWARD), "", "")
        };
        return transaction(memo, coin(0), {}, outputs);
    }

    void _sort() noexcept {
        _blocks.sort([](const core::block &a, const core::block &b) {
            return a.get_number() > b.get_number();
        });
    }

private:
    std::forward_list<block> _blocks;
};

}






// if (not b.is_valid()) {
        //     throw std::runtime_error("my error 32");
        // }
        // _write_block(b);
        // if (length() > BLOCK_MAX_SIZE) {
        //     _blocks.pop_back();
        // }


// private:
//     std::vector<std::string> _files_list(const std::string &path) const noexcept {
//         auto p = fs::path(path)
//         auto v = std::vector<std::string>();
//         if (fs::is_directory(p)) {
//             for (auto &i : fs::directory_iterator(p)) {
//                 v.push_back(i.path())
//             }
//         }
//         return v;
//     }

//     block _read_block(const std::string &path) const noexcept {
//         auto file = std::ifstream(path);
//         if (file.is_open()) {
//             auto data = std::string();

//             auto line = std::string();
//             while (std::getline(file, line)) {
//                 data += line;
//             }

//             return block(json_t::parse(data));
//         }
//     }

//     void _write_block(const block &b) const noexcept {
//         auto path = std::string(CHAIN_DEFAULT_PATH) + "/" + std::to_string(b.number()) + ".json";
//         auto file = std::ofstream(path);
//         if (file.is_open()) {
//             file << b;
//             file.close();
//         }
    // }


// public:
//     const block * find_block_by_hash(const hash_t &hash) const noexcept {
//         const auto itr = std::find_if(_blocks.begin(), _blocks.end(), [&](const auto &item) {
//             return item.hash() == hash;
//         });
//         if (itr != _blocks.end()) {
//             return &(*itr);
//         }
//         return nullptr;
//     }

//     const transaction * find_transaction_by_hash(const hash_t &hash) const noexcept {
//         for (const auto &block : _blocks) {
//             const auto itr = block.find_by_hash(hash);
//             if (itr != nullptr) {
//                 return itr;
//             }
//         }
//         return nullptr;
//     }
