#ifndef PACKET__PACKET_SERVICE_HPP
#define PACKET__PACKET_SERVICE_HPP

#include <Absolute.hpp>
#include <AbsoluteEntity.hpp>
#include <Binary.hpp>
#include <BinaryEntity.hpp>
#include <Ethernet.hpp>
#include <EthernetEntity.hpp>
#include <Ipv4.hpp>
#include <Ipv4Entity.hpp>
#include <StackableEntity.hpp>
#include <Udp.hpp>
#include <UdpEntity.hpp>
#include <Utility.hpp>
#include <vector>

namespace Packet
{
class PacketService
{
  public:
    static StackablePtr StackableFromEntity(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
