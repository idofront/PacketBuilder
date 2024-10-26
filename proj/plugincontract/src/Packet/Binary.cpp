#include <Packet/Binary.hpp>

namespace PluginContract
{
namespace Packet
{
Binary::Binary(std::size_t length) : Stackable(length)
{
}

Binary::Binary(std::vector<uint8_t> data) : Stackable(data.size())
{
    std::copy(data.begin(), data.end(), this->DataArray().get());
}

Binary::Binary(uint8_t *data, std::size_t length) : Binary(std::vector<uint8_t>(data, data + length))
{
}

Binary::~Binary()
{
}
} // namespace Packet
} // namespace PluginContract
