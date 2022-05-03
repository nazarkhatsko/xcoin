#pragma once

#include <rpc/client.h>
#include <json.hpp>
using namespace nlohmann;

#include <string>

namespace network {

class client: public rpc::client {
public:
    explicit client(const std::string &address, std::uint16_t port):
        _address   (address),
        _port      (port),
        rpc::client(address, port)
    {}

    explicit client(const json &j):
        _address   (j["address"]),
        _port      (j["port"]),
        rpc::client(j["address"], j["port"])
    {}

public:
    inline std::string   get_address() const noexcept { return _address; }
    inline std::uint16_t get_port()    const noexcept { return _port; }

public:
    bool operator==(const client &c) const noexcept { return to_json() == c.to_json(); }
    bool operator==(const json &j)   const noexcept { return to_json() == j; }

    bool operator!=(const client &c) const noexcept { return not (*this == c); }
    bool operator!=(const json &j)   const noexcept { return not (*this == j); }

    client & operator=(const client &c) noexcept { return (*this = c); }
    client & operator=(const json &j)   noexcept { return (*this = client(j)); }

    operator json() const { return to_json(); }

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
};

}
