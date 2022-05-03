#pragma once

#include "core/output.hpp"

#include <map>
#include <string>
#include <vector>
#include <algorithm>

namespace core {

class utxo {
public:
    explicit utxo() {}

    explicit utxo(const std::list<core::output> &outputs):
        _outputs(outputs)
    {}

    explicit utxo(const json &j) {
        for (const auto &i : j) {
            _outputs.push_back(core::output(i));
        }
    }

public:
    bool operator==(const utxo &ux) const noexcept {
        return true; // fix
    }

    bool operator!=(const utxo &ux) const noexcept {
        return not (*this == ux);
    }

    utxo & operator=(const json &j) noexcept {
        return (*this = utxo(j));
    }

    operator json() const {
        return to_json();
    }

public:
    inline bool empty() const noexcept {
        return _outputs.empty();
    }

    void push(const core::output &o) {
        _outputs.push_back(o);
    }

    void pop(const std::string &hash) {
        _outputs.remove_if([&](const core::output &o) {
            return o.hash() == hash;
        });
    }

    void clear() noexcept {
        _outputs.clear();
    }

    const core::output * find(const std::string &hash) const noexcept {
        const auto itr = std::find_if(_outputs.begin(), _outputs.end(), [&](const core::output &o) {
            return o.hash() == hash;
        });
        if (itr != _outputs.end()) {
            return &(*itr);
        }
        return nullptr;
    }

    std::list<core::output>::iterator begin() noexcept { return _outputs.begin(); }
    std::list<core::output>::iterator end() noexcept   { return _outputs.end(); }

    std::list<core::output>::const_iterator begin() const noexcept { return _outputs.begin(); }
    std::list<core::output>::const_iterator end() const noexcept   { return _outputs.end(); }

// public:
    // void all_coins()

public:
    std::size_t size() const {
        return sizeof(*this);
    }

    std::size_t length() const {
        return _outputs.size();
    }

public:
    json to_json() const {
        auto j = json::array();
        for (const auto &i : _outputs) {
            j.push_back(i);
        }
        return j;
    }

private:
    std::list<core::output> _outputs;
};

}


    // explicit utxo(const std::map<std::string, std::vector<core::output>> &outputs):
    //     _outputs(outputs)
    // {}

    // explicit utxo(const json &j) {
    //     for (const auto &i : j) {
    //         for (const auto &j : i["outputs"]) {
    //             _outputs[i["address"]].push_back(core::output(j));
    //         }
    //     }
    // }



    // std::map<std::string, std::vector<core::output>> _outputs;


// for (const auto &[key, value] : _outputs) {
        //     j.push_back({
        //         {"address", key},
        //         {"outputs", std::accumulate(value.begin(), value.end(), json(), [](json j, core::output o) { j.push_back(o); return j; })}
        //     });
        // }


    // void push_output(const std::string &address, const core::output &o) noexcept {
    //     if (_outputs.find(address) == _outputs.end()) {
    //         _outputs[address] = std::vector<core::output>();
    //     }
    //     _outputs[address].push_back(o);
    // }

    // const core::transaction * find(const std::string &hash) {
    //     const itr = std::find_if(_transactions.begin(), _transactions.end(), [](const core::transaction &t) {
    //         return t.hash() == hash;
    //     });
    //     if (itr != _transactions.end()) {
    //         return itr;
    //     }
    //     return nullptr;
    // }

    // std::vector<core::output> operator[](const std::string &address) noexcept {
    //     if (_outputs.find(address) == _outputs.end()) {
    //         return std::vector<core::output>();
    //     }
    //     return _outputs[address];
    // }

    // std::map<std::string, std::vector<core::output>> get_outputs() {
    //     return _outputs;
    // }

