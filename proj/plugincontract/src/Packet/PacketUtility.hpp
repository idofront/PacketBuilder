#ifndef PLUGIN_CONTRACT__PACKET_UTILITY_HPP__
#define PLUGIN_CONTRACT__PACKET_UTILITY_HPP__

#include <Packet/Stackable.hpp>
#include <exception/InvalidArgumentException.hpp>
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

/// @brief いくつ Stack されているかを取得する．
/// @param stackable
/// @return Stack を持たない場合は 0 を返す．
inline uint64_t Depth(StackablePtr stackable)
{
    if (stackable == nullptr)
    {
        throw Utility::Exception::InvalidArgumentException("The stackable is nullptr.");
    }
    return stackable->Stack.Value() ? Depth(stackable->Stack.Value()) + 1 : 0;
}

/// @brief Stack の末尾を取得する．
/// @param stackable
/// @param depth
/// @return 末尾から depth 番目の Stackable を返す．
inline StackablePtr Tail(StackablePtr stackable, uint64_t depth = 1)
{
    if (stackable == nullptr)
    {
        throw Utility::Exception::InvalidArgumentException("The stackable is nullptr.");
    }

    auto currentDepth = Depth(stackable) + 1;
    if (currentDepth < depth)
    {
        auto fmt =
            boost::format("The requested depth is too deep. The current depth is %1%, but the requested depth is %2%. "
                          "Returning the current stackable.");
        auto message = fmt % currentDepth % depth;
        SPDLOG_WARN(message.str());
    }

    if (currentDepth > depth)
    {
        return stackable->Stack.Value() ? Tail(stackable->Stack.Value(), depth - 1) : stackable;
    }

    return stackable;
}

} // namespace Packet
} // namespace PluginContract

#endif
