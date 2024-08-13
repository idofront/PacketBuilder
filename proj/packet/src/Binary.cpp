#include <Binary.hpp>

namespace Packet
{
Binary::Binary(std::size_t length) : Stackable(length)
{
}

Binary::~Binary()
{
}
} // namespace Packet
