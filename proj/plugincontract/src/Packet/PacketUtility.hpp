#ifndef PLUGIN_CONTRACT__PACKET_UTILITY_HPP__
#define PLUGIN_CONTRACT__PACKET_UTILITY_HPP__

#include <Packet/Stackable.hpp>
#include <memory>

namespace PluginContract
{
namespace Packet
{

inline std::string ToHex(uint8_t value)
{
    return std::string(1, "0123456789ABCDEF"[value >> 4]) + std::string(1, "0123456789ABCDEF"[value & 0x0F]);
}

inline std::string HexDump(StackablePtr stackable)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    std::string hexDump;
    const uint8_t *data = stackable->DataArray().get();
    std::size_t length = stackable->Length();

    for (std::size_t i = 0; i < length; ++i)
    {
        hexDump += ToHex(data[i]) + " ";

        if ((i + 1) % 16 == 0)
        {
            if (i + 1 < length)
            {
                hexDump += "\n";
            }
        }
    }

    return hexDump;
}

inline StackablePtr Tail(StackablePtr stackable)
{
    return stackable->Stack.Value() ? Tail(stackable->Stack.Value()) : stackable;
}

} // namespace Packet
} // namespace PluginContract

#endif
