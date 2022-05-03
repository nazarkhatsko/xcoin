#pragma once

#include "defines.hpp"
#include "args/ioption.hpp"
#include "network/network.hpp"

#include <iostream>

namespace args {

class wallet: public ioption {
public:
    int _main(int argc, char **argv) override {
        auto desc = po::options_description(std::string(NAME) + " Wallet Options");
        desc.add_options()
            ("private-key", po::value<std::string>(),   "Private key in WIF format to import")
            ("port",        po::value<std::uint16_t>(), "Private key in WIF format to import")
            ("node",        po::value<std::string>(),   "Private key in WIF format to import")
            ("help,h",                                  "Produce help message")
        ;
        auto vm = po::variables_map();
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        if (vm.count("private-key") and vm.count("port") and vm.count("node")) {
            auto private_key  = vm["private-key"].as<std::string>();
            auto port         = vm["port"].as<std::uint16_t>();
            auto node_params  = vm["node"].as<std::string>();
            auto i            = node_params.find(":");
            auto node_address = node_params.substr(0, i);
            auto node_port    = std::stol(node_params.substr(i + 1));
            auto node         = network::node(port, {node_address, node_port});

            return _run_cil(private_key, node);
        }

        std::cout << "A subcommand is required\n";
        std::cout << "Run with -h [ --help ] for more information\n";

        return EXIT_FAILURE;
    }

    int _run_cil(const std::string &private_key, network::node &node) {
        node.set_validator_new_transaction([](const core::utxo &ux, const core::transaction &nt) {
            // is valid
            return true;
        });

        node.set_validator_new_block([](const core::chain &c, const core::utxo &ux, const core::block &nb) {
            // is valid
            return true;
        });

        node.run();
        std::string str;
        while (str != "q") {
            std::cout << "Choice:\n" \
                "[q]uit\n" \
                "[c]lients\n" \
                "[t]send_transaction\n" \
                "[b]send_block\n" \
                "[u]txo\n" \
                "[m]empool\n" \
                "[ch]count_chain\n" \
                "[h]chain\n\n";
            std::cout << "Enter => ";
            std::cin >> str;

            if (str == "c") {
                for (const auto &i : node._clients) {
                    std::cout << i->to_json().dump(2) << "\n";
                }
            } else if (str == "t") {
                auto t = core::transaction("", core::coin(10), {}, {});
                node.send_new_transaction(t);
            } else if (str == "b") {
                auto b = core::block(0, "ffd", {});
                node.send_new_block(b);
            } else if (str == "u") {
                std::cout << node._utxo.to_json().dump(2) << "\n";
            } else if (str == "m") {
                std::cout << node._mempool.to_json().dump(2) << "\n";
            } else if (str == "h") {
                std::cout << node._chain.to_json().dump(2) << "\n";
            } else if (str == "ch") {
                std::cout << node._chain.length() << "\n";
            }

            std::cout << "\n\n";
        }
        node.quit();

        return EXIT_SUCCESS;
    }
};

}
