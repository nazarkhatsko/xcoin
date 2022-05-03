#include "crypto/base.hpp"

#define BOOST_TEST_MODULE base_test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(base58_test) {
    auto a = crypto::base58_encode("test string");
    auto b = std::string("Vs5LyRhXt9nUp14");
    auto m = std::string("the result is unplanned");

    BOOST_CHECK_MESSAGE(a == b, m);
}
