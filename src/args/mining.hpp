#pragma once

#include "defines.hpp"
#include "args/ioption.hpp"
#include "network/network.hpp"
#include "consensus/consensus.hpp"
#include "core/core.hpp"
#include "crypto/crypto.hpp"

#include <iostream>

namespace args {

class mining: public ioption {
    int _main(int argc, char **argv) override {
        auto desc = po::options_description(std::string(NAME) + " Mining Options");
        desc.add_options()
            ("address", po::value<std::string>(),   "Private key in WIF format to import")
            ("port",    po::value<std::uint16_t>(), "Private key in WIF format to import")
            ("node",    po::value<std::string>(),   "Private key in WIF format to import")
            ("help,h",                              "Produce help message")
        ;
        auto vm = po::variables_map();
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        if (vm.count("address") and vm.count("port")) {
            auto address  = vm["address"].as<std::string>();
            auto port     = vm["port"].as<std::uint16_t>();

            if (vm.count("node")) {
                auto node_params  = vm["node"].as<std::string>();
                auto i            = node_params.find(":");
                auto node_address = node_params.substr(0, i);
                auto node_port    = std::stol(node_params.substr(i + 1));
                auto node         = network::node(port, {node_address, node_port});

                return _run_cil(address, node);
            } else {
                auto node = network::node(port);

                return _run_cil(address, node);
            }
        }

        std::cout << "A subcommand is required\n";
        std::cout << "Run with -h [ --help ] for more information\n";

        return EXIT_FAILURE;
    }

    int _run_cil(const std::string &address, network::node &node) {
        node.set_validator_new_transaction([&](const core::utxo &ux, const core::transaction &nt) {
            // for (const auto &i : nt.get_inputs()) {
            //     const auto *itr = ux.find(i.hash());
            //     if (itr == nullptr) {
            //         return false;
            //     }
            // }
            return true;
        });

        node.set_validator_new_block([&](const core::chain &c, const core::utxo &ux, const core::block &nb) {
            if ((c.empty() and nb.get_number() != 0) or (not c.empty() and nb.get_number() != c.begin()->get_number() + 1)) {
                return false;
            }

            if (not c.empty() and c.top().hash() != nb.get_previous_block()) {
                return false;
            }

            if (nb.begin()->outputs.begin()->get_coins() != core::coin(MINE_REWARD)) {
                return false;
            }

            const auto jnb = nb.to_json();
            std::cout << "Number: "             << jnb["number"].get<std::size_t>()             << "\n";
            std::cout << "Hash: "               << jnb["hash"].get<std::string>()               << "\n";
            std::cout << "Merkle root: "        << jnb["merkle_root"].get<std::string>()        << "\n";
            std::cout << "Previous block: "     << jnb["previous_block"].get<std::string>()     << "\n";
            std::cout << "Timestamp: "          << jnb["timestamp"].get<std::time_t>()          << "\n";
            std::cout << "Transactions count: " << jnb["transactions_count"].get<std::size_t>() << "\n";
            std::cout << "Miner: " << nb.begin()->outputs.begin()->get_address() << "\n";
            std::cout << "\n";

            return true;
        });

        node.set_miner([&](const core::chain &c, const core::mempool &mp) { // fix
            std::size_t i = 0;
            auto b = c.create_new_block(address);
            // while (not core::block_is_done(b)) {
            //     // if (not mp.empty() and i < mp.length()) {
            //     //     b.push(mp[i]);
            //     //     i++;
            //     // } else {
            //     //     i = 0;
            //     // }
            // }

            // core::block_validing(b);
            node.send_new_block(b);
        });

        node.run();
        std::string str;
        while (str != "q") {
            std::cin >> str;
        }
        node.quit();

        return EXIT_SUCCESS;
    }
};

}





// auto b = c.create_new_block(address);
// auto ms = uid(mt);
// std::cout << "time: " << ms << " - " << "number: " << b.get_number() << " - " << "lenght: " << c.length() << "\n";
// for (const auto &i : c.get_blocks()) {
//     std::cout << i.get_number() << " ";
// }
// std::cout << "\n\n";
// tools::sleep(ms);


// auto rd = std::random_device();
// auto mt = std::mt19937(rd());
// auto uid = std::uniform_real_distribution<double>(0.0, 10000.0);



            // std::cout << b.get_number() << "\n";
// if (not c.empty()) {
//     std::cout << c.front().get_number() << "\n";
// }
// std::cout << c.length() << " " << b.get_number() << "\n";
// tools::sleep(2000);

// auto b = c.create_new_block(address);
// auto ms = uid(mt);
// std::cout << "tile: " << ms << " - " << "number: " << b.get_number() << " - " << "lenght: " << c.length() << "\n";
// tools::sleep(ms);
