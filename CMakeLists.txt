cmake_minimum_required(VERSION "3.8")

set(TARGET "xcoin")
set(PROJECT_SOURCE_DIR "/Users/nkhatsko/Desktop/xcoin3")
set(CMAKE_CXX_STANDARD "17")

set(RPCLIB_INCLUDE_DIR "/Users/nkhatsko/Desktop/xcoin/libraries/rpc/include")
set(RPCLIB_LIBRARIES "/Users/nkhatsko/Desktop/xcoin/libraries/rpc/lib/librpc.a")

set(SECP256K1_INCLUDE_DIR "/Users/nkhatsko/Desktop/xcoin/libraries/secp256k1/include")
set(SECP256K1_LIBRARIES "/Users/nkhatsko/Desktop/xcoin/libraries/secp256k1/lib/libsecp256k1.a")

project(${TARGET})

include_directories(
    "${PROJECT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE SOURCE
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

find_package(Boost "1.76.0" REQUIRED COMPONENTS
    thread
    program_options
)
find_package(OpenSSL REQUIRED)
# find_package(rpclib REQUIRED)

set(INCLUDE_DIRS
    ${Boost_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${RPCLIB_INCLUDE_DIR}
    ${SECP256K1_INCLUDE_DIR}
)

set(LIBRARIES
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${RPCLIB_LIBRARIES}
    ${SECP256K1_LIBRARIES}
)

include_directories(${INCLUDE_DIRS})
add_executable(${TARGET} ${SOURCE})
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})


# test
list(REMOVE_ITEM SOURCE "/Users/nkhatsko/Desktop/xcoin/src/main.cpp")

enable_testing()
find_package(Boost "1.76.0" REQUIRED COMPONENTS
    unit_test_framework
)

file(GLOB_RECURSE TEST_SOURCE
    "${PROJECT_SOURCE_DIR}/test/*.cpp"
)

foreach(UT_SRC ${TEST_SOURCE})
    get_filename_component(UT_BIN ${UT_SRC} NAME)
    string(REPLACE ".cpp" "" UT_BIN ${UT_BIN})
    add_executable(${UT_BIN} ${UT_SRC} ${SOURCE})
    target_link_libraries(${UT_BIN} PUBLIC ${LIBRARIES} Boost::unit_test_framework)
    add_test("${UT_BIN}_test" ${UT_BIN})
endforeach()
