#include "args/args.hpp"
#include "network/network.hpp"
#include "consensus/consensus.hpp"
#include "core/core.hpp"
#include "crypto/crypto.hpp"

#include <iostream>
#include <memory>

int main(int argc, char **argv) {
    auto prog = args::program(argc, argv);

    prog.add_option("wallet", std::shared_ptr<args::ioption>(new args::wallet));
    prog.add_option("mining", std::shared_ptr<args::ioption>(new args::mining));
    prog.add_option("tools",  std::shared_ptr<args::ioption>(new args::tools));

    return prog.run();
}
