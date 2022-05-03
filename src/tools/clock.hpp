#pragma once

#include <chrono>
#include <thread>

namespace tools {

std::time_t clock_now() {
    return std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
}

void sleep(std::time_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

}
