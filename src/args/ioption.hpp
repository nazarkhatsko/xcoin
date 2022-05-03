#pragma once

// #include <boost/format.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace args {

class ioption {
public:
    explicit ioption() {}
    virtual ~ioption() {}

public:
    virtual int operator()(int argc, char **argv) {
        return _main(argc, argv);
    }

protected:
    virtual int _main(int argc, char **argv) = 0;
};

}
