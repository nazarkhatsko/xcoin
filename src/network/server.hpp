#pragma once

#include <boost/thread.hpp>
#include <rpc/server.h>
#include <json.hpp>
using namespace nlohmann;

#include <string>

namespace network {

class server: public rpc::server {
public:
    explicit server(std::uint16_t port):
        _address   ("127.0.0.1"),
        _port      (port),
        rpc::server(port)
    {}

    explicit server(const json &j):
        _address   (j["address"]),
        _port      (j["port"]),
        rpc::server(j["port"])
    {}

public:
    inline std::string   get_address() const noexcept { return _address; }
    inline std::uint16_t get_port()    const noexcept { return _port; }

public:
    bool operator==(const server &s) const noexcept { return to_json() == s.to_json(); }
    bool operator==(const json &j)   const noexcept { return to_json() == j; }

    bool operator!=(const server &s) const noexcept { return not (*this == s); }
    bool operator!=(const json &j)   const noexcept { return not (*this == j); }

    server & operator=(const server &s) noexcept { return (*this = s); }
    server & operator=(const json &j)   noexcept { return (*this = server(j)); }

    operator json() const { return to_json(); }

public:
    void start() noexcept {
        _thread = boost::thread([&]() { run(); });
    }

    void stop() noexcept {
        _thread.interrupt();
    }

public:
    json to_json() const {
        auto j = json({
            {"address", _address},
            {"port",    _port}
        });
        return j;
    }

private:
    std::string   _address;
    std::uint16_t _port;
    boost::thread _thread;
};

}
