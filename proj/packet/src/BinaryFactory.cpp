#include <BinaryFactory.hpp>

namespace Packet
{
StackablePtr BinaryFactory::Get(PacketEntity::StackableEntityPtr entity)
{
    auto binaryEntityPtr = std::dynamic_pointer_cast<PacketEntity::BinaryEntity>(entity);
    auto entityPtr = std::make_shared<Binary>(binaryEntityPtr);
    return entityPtr;
}
} // namespace Packet
