#include <AbsoluteFactory.hpp>

namespace Packet
{
StackablePtr AbsoluteFactory::Get(PacketEntity::StackableEntityPtr entity)
{
    auto entityPtr = std::dynamic_pointer_cast<PacketEntity::AbsoluteEntity>(entity);
    return std::make_shared<Absolute>(entityPtr);
}
} // namespace Packet
