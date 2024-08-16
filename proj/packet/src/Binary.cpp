#include <Binary.hpp>
#include <BinaryEntity.hpp>

namespace Packet
{
Binary::Binary(std::size_t length) : Stackable(length, std::make_shared<PacketEntity::BinaryEntity>(length))
{
}

Binary::Binary(PacketEntity::BinaryEntityPtr entity) : Stackable(entity->Data->size(), entity)
{
}

Binary::~Binary()
{
}
} // namespace Packet
