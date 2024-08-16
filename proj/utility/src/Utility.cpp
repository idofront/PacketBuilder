#include <Utility.hpp>

namespace Utility
{
std::string Demangle(const std::string &demangle)
{
    int status;
    auto demangledName = abi::__cxa_demangle(demangle.c_str(), 0, 0, &status);
    if (status != 0)
    {
        throw std::runtime_error("Failed to demangle name");
    }
    auto demangledNameAsString = std::string(demangledName);
    delete demangledName;
    return demangledNameAsString;
}

std::string HexDump(DataArrayPtr data, std::size_t length)
{
    std::string hexDump;
    hexDump.reserve(length * 2);

    for (std::size_t i = 0; i < length; ++i)
    {
        hexDump += "0123456789ABCDEF"[data[i] >> 4];
        hexDump += "0123456789ABCDEF"[data[i] & 0x0F];
    }

    return hexDump;
}

void EthernetAddressToString(std::string &addr, const uint8_t *const ether_addr)
{
    char buf[18];
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", ether_addr[0], ether_addr[1], ether_addr[2], ether_addr[3],
            ether_addr[4], ether_addr[5]);
    addr = std::string(buf);
}

void EthernetAddressFromString(const std::string &addr, uint8_t *const ether_addr)
{
    std::sscanf(addr.c_str(), "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", &ether_addr[0], &ether_addr[1],
                &ether_addr[2], &ether_addr[3], &ether_addr[4], &ether_addr[5]);
}
} // namespace Utility
