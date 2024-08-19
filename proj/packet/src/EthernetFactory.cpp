#include <EthernetFactory.hpp>

namespace Packet
{
StackablePtr EthernetFactory::Get(PacketEntity::StackableEntityPtr entity)
{
    auto entityPtr = std::dynamic_pointer_cast<PacketEntity::EthernetEntity>(entity);
    return std::make_shared<Ethernet>(entityPtr);
}
} // namespace Packet
