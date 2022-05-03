#pragma once

#include "defines.hpp"
#include "args/ioption.hpp"
#include "crypto/crypto.hpp"

#include <iostream>

namespace args {

class tools: public ioption {
    int _main(int argc, char **argv) override {
        auto desc = po::options_description(std::string(NAME) + " Tools Options");
        desc.add_options()
            ("generate-keys", "Generate keys")
            ("help,h",        "Produce help message")
        ;
        auto vm = po::variables_map();
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        if (vm.count("generate-keys")) {
            return _generate_keys();
        }

        std::cout << "A subcommand is required\n";
        std::cout << "Run with -h [ --help ] for more information\n";

        return EXIT_FAILURE;
    }

    int _generate_keys() {
        auto ecdsa       = crypto::ecdsa();
        auto private_key = ecdsa.private_key();
        auto public_key  = ecdsa.public_key(private_key);
        auto address     = crypto::base58_encode(crypto::bytes_to_hex(public_key));

        std::cout << "Address: "     << address                           << "\n";
        std::cout << "Public key: "  << crypto::bytes_to_hex(public_key)  << "\n";
        std::cout << "Private key: " << crypto::bytes_to_hex(private_key) << "\n";

        return EXIT_SUCCESS;
    }
};

}
