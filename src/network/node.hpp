#pragma once

#include "core/utxo.hpp"
#include "core/mempool.hpp"
#include "core/chain.hpp"
#include "core/block.hpp"
#include "core/transaction.hpp"
#include "network/miner.hpp"
#include "network/client.hpp"
#include "network/server.hpp"

#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <future>

namespace network {

class node {
public:
    explicit node(std::uint16_t port):
        _server(port)
    {
        _binds_calls();
    }

    explicit node(std::uint16_t port, std::pair<std::string, std::uint16_t> params):
        _server(port)
    {
        _binds_calls();

        _clients.push_back(new client(params.first, params.second));
        auto clients = json::parse(_clients[0]->call("get_clients").as<std::string>());
        for (const auto &i : clients) {
            _clients.push_back(new client(i));
        }
        _chain   = json::parse(_clients[0]->call("get_blocks").as<std::string>());
        _utxo    = json::parse(_clients[0]->call("get_utxo").as<std::string>());
        _mempool = json::parse(_clients[0]->call("get_mempool").as<std::string>());
    }

    virtual ~node() {
        for (auto &i : _clients) {
            delete i;
        }
    }

public:
    void set_miner(std::function<void(const core::chain &, const core::mempool &)> mining) noexcept {
        _miner.set(std::bind(mining, std::cref(_chain), std::cref(_mempool)));
    }

    void set_validator_new_transaction(std::function<bool(const core::utxo &, const core::transaction &)> validator_new_transaction) noexcept {
        _validator_new_transaction = validator_new_transaction;
    }

    void set_validator_new_block(std::function<bool(const core::chain &, const core::utxo &, const core::block &)> validator_new_block) noexcept {
        _validator_new_block = validator_new_block;
    }

public:
    void run() noexcept {
        for (const auto &i : _clients) {
            i->call("connection", _server.to_json().dump());
        }
        _server.start();

        if (_miner) {
            _miner.start();
        }
    }

    void quit() noexcept {
        for (const auto &i : _clients) {
            i->call("disconnection", _server.to_json().dump());
        }
        _server.stop();

        if (_miner) {
            _miner.stop();
        }
    }

public:
    void send_new_transaction(const core::transaction &t) noexcept {
        if (_validator_new_transaction(_utxo, t)) {
            _mempool.push(t);
            for (const auto &i : _clients) {
                i->call("send_new_transaction", t.to_json().dump());
            }
        }
    }

    void send_new_block(const core::block &b) noexcept {
        if (_validator_new_block(_chain, _utxo, b)) {
            _set_new_block(b);

            if (_miner) {
                _miner.restart();
            }

            for (const auto &i : _clients) {
                i->call("send_new_block", b.to_json().dump());
            }
        }
    }

private:
    void _connection(std::string sc) {
        auto jc = json::parse(sc);
        _clients.push_back(new client(jc));
    }

    void _disconnection(std::string sc) {
        auto jc = json::parse(sc);
        auto itr = std::find_if(_clients.begin(), _clients.end(), [&](const auto &i) {
            return i->get_address() == jc["address"] and i->get_port() == jc["port"];
        });
        if (itr != _clients.end()) {
            _clients.erase(itr);
        }
    }

    std::string _get_clients() {
        auto clients = json::array();
        for (const auto &i : _clients) {
            clients.push_back(i->to_json());
        }
        return clients.dump();
    }

    std::string _get_blocks() {
        return _chain.to_json().dump();
    }

    std::string _get_utxo() {
        return _utxo.to_json().dump();
    }

    std::string _get_mempool() {
        return _mempool.to_json().dump();
    }

    void _send_new_transaction(std::string st) {
        const auto t = core::transaction(json::parse(st));
        if (_validator_new_transaction(_utxo, t)) {
            _mempool.push(t);
        }
    }

    void _send_new_block(std::string sb) {
        const auto b = core::block(json::parse(sb));
        if (_validator_new_block(_chain, _utxo, b)) {
            _set_new_block(b);
            if (_miner) {
                _miner.restart();
            }
        }
    }

private:
    void _binds_calls() noexcept {
        _server.bind("connection",    [&](std::string sc) { std::mem_fn(&node::_connection)(this, sc); });
        _server.bind("disconnection", [&](std::string sc) { std::mem_fn(&node::_disconnection)(this, sc); });

        _server.bind("get_clients", [&]() { return std::mem_fn(&node::_get_clients)(this); });
        _server.bind("get_blocks",  [&]() { return std::mem_fn(&node::_get_blocks)(this); });
        _server.bind("get_utxo",    [&]() { return std::mem_fn(&node::_get_utxo)(this); });
        _server.bind("get_mempool", [&]() { return std::mem_fn(&node::_get_mempool)(this); });

        _server.bind("send_new_transaction", [&](std::string st) { std::mem_fn(&node::_send_new_transaction)(this, st); });
        _server.bind("send_new_block",       [&](std::string sb) { std::mem_fn(&node::_send_new_block)(this, sb); });
    }

    void _set_new_block(const core::block &b) noexcept {
        _chain.push(b);

        // for (const auto &i : b) {
        //     for (const auto &j : i.get_inputs()) {
        //         // _utxo.push_output(j);
        //     }

        //     for (const auto &j : i.get_outputs()) {
        //         // _utxo.push_output(j);
        //     }

        //     // _mempool.pop_transaction(i.hash());
        // }
    }

public:
    miner                 _miner;
    server                _server;
    std::vector<client *> _clients;
    core::chain           _chain;
    core::utxo            _utxo;
    core::mempool         _mempool;

private:
    std::function<bool(const core::utxo &, const core::transaction &)>                _validator_new_transaction;
    std::function<bool(const core::chain &, const core::utxo &, const core::block &)> _validator_new_block;
};

}
