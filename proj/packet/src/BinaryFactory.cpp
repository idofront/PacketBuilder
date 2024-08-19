#include <BinaryFactory.hpp>

namespace Packet
{
StackablePtr BinaryFactory::Get(PacketEntity::StackableEntityPtr entity)
{
    auto entityPtr = std::dynamic_pointer_cast<PacketEntity::BinaryEntity>(entity);
    return std::make_shared<Binary>(entityPtr);
}
} // namespace Packet
