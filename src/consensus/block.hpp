#pragma once

#include "defines.hpp"
#include "core/core.hpp"
#include "tools/tools.hpp"

namespace consensus {

namespace block {

    bool is_done(const core::block &b) {
        return b.size() >= BLOCK_MAX_SIZE or b.get_timestamp() + BLOCK_MAX_TIME <= tools::clock_now();
    }

    bool is_valid(const core::block &b) {
        return b.hash().substr(0, MINE_DIFFICULTY) == std::string(MINE_DIFFICULTY, MINE_BITS);
    }

    void validing(core::block &b) {
        while (not is_valid(b)) {
            b++;
        }
    }

}

}
