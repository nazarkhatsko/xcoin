#pragma once

#include "crypto/sha.hpp"

#include <vector>
#include <string>

namespace crypto {

class merkletree {
public:
    explicit merkletree(): _data({}) {}

    explicit merkletree(const std::vector<std::string> &data): _data(data) {}

public:
    auto root() const noexcept -> std::string {
        auto tree = _hasing(_data);
        while (tree.size() > 1) {
            tree = _marge(tree);
        }
        return tree.front();
    }

    auto insert(const std::string &s) noexcept -> void {
        _data.push_back(s);
    }

private:
    auto _marge(const std::vector<std::string> &v) const noexcept -> std::vector<std::string> {
        auto v2 = std::vector<std::string>();
        for (int i = 0; i < v.size(); i += 2) {
            if (i == v.size() - 1) {
                v2.push_back(crypto::sha256(v[i]));
            } else {
                v2.push_back(crypto::sha256(v[i] + v[i + 1]));
            }
        }
        return v2;
    }

    auto _hasing(std::vector<std::string> v) const noexcept -> std::vector<std::string> {
        for (auto &i : v) {
            i = crypto::sha256(i);
        }
        return v;
    }

private:
    std::vector<std::string> _data;
};

}
