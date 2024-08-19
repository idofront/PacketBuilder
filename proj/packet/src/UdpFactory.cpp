#include <UdpFactory.hpp>

namespace Packet
{
StackablePtr UdpFactory::Get(PacketEntity::StackableEntityPtr entity)
{
    auto entityPtr = std::dynamic_pointer_cast<PacketEntity::UdpEntity>(entity);
    return std::make_shared<Udp>(entityPtr);
}
} // namespace Packet
