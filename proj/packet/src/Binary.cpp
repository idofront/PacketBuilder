#include <Binary.hpp>
#include <BinaryEntity.hpp>

namespace Packet
{
Binary::Binary(std::size_t length) : Stackable(length, std::make_shared<PacketEntity::BinaryEntity>(length))
{
}

Binary::Binary(PacketEntity::BinaryEntityPtr entity) : Stackable(entity->Data->size(), entity)
{
    auto length = entity->Data->size();
    for (std::size_t i = 0; i < length; i++)
    {
        this->DataArray()[i] = entity->Data->at(i);
    }
}

Binary::~Binary()
{
}
} // namespace Packet
