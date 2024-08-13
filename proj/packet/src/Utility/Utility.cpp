#include <Utility/Utility.hpp>
#include <map>

namespace Packet::Utility
{
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

} // namespace Packet::Utility
