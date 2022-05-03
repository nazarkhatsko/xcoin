#pragma once

#include "args/ioption.hpp"

#include <map>
#include <string>

namespace args {

class program {
public:
    explicit program(int argc, char **argv):
        _argc(argc),
        _argv(argv)
    {}

public:
    void add_option(const std::string &key, const std::shared_ptr<ioption> &option) {
        _options[key] = option;
    }

    int run() {
        const auto key = std::string(_argv[1]);
        _argv[1] = _argv[0];
        _argc--;
        _argv++;

        return (*_options[key])(_argc, _argv);
    }

private:
    int                                             _argc;
    char                                          **_argv;
    std::map<std::string, std::shared_ptr<ioption>> _options;
};

}
