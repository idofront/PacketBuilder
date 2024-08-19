#include <Ipv4Factory.hpp>

namespace Packet
{
StackablePtr Ipv4Factory::Get(PacketEntity::StackableEntityPtr entity)
{
    auto entityPtr = std::dynamic_pointer_cast<PacketEntity::Ipv4Entity>(entity);
    return std::make_shared<Ipv4>(entityPtr);
}
} // namespace Packet
