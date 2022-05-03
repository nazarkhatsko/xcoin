#pragma once

#include "core/transaction.hpp"

#include <list>
#include <algorithm>
#include <stdexcept>

namespace core {

class mempool {
public:
    explicit mempool() {}

    explicit mempool(const std::list<core::transaction> &transactions):
        _transactions(transactions)
    {
        _sort();
    }

    explicit mempool(const json &j) {
        for (const auto &i : j) {
            _transactions.push_back(core::transaction(i));
        }
        _sort();
    }

public:
    bool operator==(const mempool &mp) const noexcept {
        return true; // fix
    }

    bool operator!=(const mempool &mp) const noexcept {
        return not (*this == mp);
    }

    mempool & operator=(const json &j) noexcept {
        return (*this = mempool(j));
    }

    const transaction & operator[](std::size_t i) const noexcept {
        auto itr = _transactions.begin();
        std::advance(itr, i);
        return *itr;
    }

    operator json() const noexcept {
        return to_json();
    }

public:
    inline bool empty() const noexcept {
        return _transactions.empty();
    }

    const core::transaction & top() const noexcept {
        return _transactions.front();
    }

    void push(const core::transaction &t) noexcept {
        _transactions.push_back(t);
        _sort();
    }

    void pop(const std::string &hash) {
        _transactions.remove_if([&](const core::transaction &t) {
            return t.hash() == hash;
        });
        _sort();
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

    std::list<transaction>::iterator begin() noexcept { return _transactions.begin(); }
    std::list<transaction>::iterator end() noexcept   { return _transactions.end(); }

    std::list<transaction>::const_iterator begin() const noexcept { return _transactions.begin(); }
    std::list<transaction>::const_iterator end() const noexcept   { return _transactions.end(); }

public:
    std::size_t size() const {
        return sizeof(*this);
    }

    std::size_t length() const {
        return std::distance(_transactions.begin(), _transactions.end());
    }

public:
    json to_json() const {
        auto j = json::array();
        for (const auto &i : _transactions) {
            j.push_back(i);
        }
        return j;
    }

private:
    void _sort() noexcept {
        _transactions.sort([](const core::transaction &a, const core::transaction &b) {
            return a.get_fee() > b.get_fee();
        });
    }

private:
    std::list<core::transaction> _transactions;
};

}
