#pragma once

#include "types.hpp"
#include "defines.hpp"
#include "core/core.hpp"
#include "network/network.hpp"

#include <iostream>
#include <string>

namespace cli {

class wallet {
public:
    explicit wallet(const network::node *node) {
        _node = node;

        _sep_begin  = "\n";
        _space_line = "  ";
        _sep_end    = "\n\n";

        std::cout << "Welcome to the " << NAME    << "\n";
        std::cout << "Coin version "   << VERSION << "\n";
        std::cout << "\n";
    }

public:
    char choice() noexcept {
        std::cout << "Please make your choice get\n";
        std::cout << "Select: [s]end [g]et [q]uit => ";

        return _input("")[0];
    }

    void incorrect() noexcept {
        std::cout << _sep_begin;
        std::cout << _space_line << "There is no such choice\n";
        std::cout << _sep_end;
    }

public:
    void mode_send() noexcept {
        std::cout << _sep_begin;

        auto to     = _input(_space_line + "Enter to: ");
        auto memo   = _input(_space_line + "Enter memo: ");
        auto amount = _input(_space_line + "Enter amount: ");

        // _node->send_new_transaction(core::transaction(
        //     "test",
        //     {core::input(0, "addr", core::coin(10), "e", "d")},
        //     {core::output(0, "addr", core::coin(30), "fdd", "d")}
        // ));

        std::cout << _space_line << "The transaction has been sent for processing\n";
        std::cout << _sep_end;
    }

    void mode_get() noexcept {
        std::cout << _sep_begin;
        std::cout << _space_line << "Address: 26JfW9Gu7dfhjQgzvrgm2stGmMY3Y2GTd959rDZHd4g5F\n";
        std::cout << _sep_end;
    }

    void mode_info() noexcept {
        std::cout << _sep_begin;
        std::cout << _space_line << "Address: 26JfW9Gu7dfhjQgzvrgm2stGmMY3Y2GTd959rDZHd4g5F\n";
        std::cout << _space_line << "Balance: 34.04343 XCoin\n";
        std::cout << _sep_end;
    }

public:
    static constexpr char quit = 'q';
    static constexpr char send = 's';
    static constexpr char get  = 'g';
    static constexpr char info = 'i';

private:
    std::string _input(const std::string &title) {
        std::cout << title;
        auto str = std::string();
        std::getline(std::cin, str);
        return str;
    }

private:
    network::node *_node;

private:
    std::string _sep_begin;
    std::string _space_line;
    std::string _sep_end;
};

}
