#pragma once

#include <boost/thread.hpp>

#include <functional>
// #include <iostream>

namespace network {

class miner {
public:
    explicit miner() {}

    explicit miner(std::function<void()> mining):
        _mining(mining)
    {}

public:
    void set(std::function<void()> mining) noexcept {
        _mining = mining;
    }

    void start() noexcept {
        // _thread.swap(boost::thread([&]() {
        //     _mining();
        // }));

        // _thread = boost::thread(std::mem_fn(&miner::_mining_wrapper), this);
        // _thread.detach();
        _thread = boost::thread(([&]() { _mining(); }));
    }

    void stop() noexcept {
        _thread.interrupt();
    }

    void restart() noexcept {
        stop();
        start();
    }

public:
    operator bool() const {
        return static_cast<bool>(_mining);
    }

// private:
//     void _mining_wrapper() {
//         _mining();
    // }

private:
    boost::thread         _thread;
    std::function<void()> _mining;
};

}
