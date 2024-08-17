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
/// @brief StackableEntity から Stackableを生成する
class PacketService
{
  public:
    /// @brief StackableEntity から Stackableを生成する
    /// @param entity StackableEntity
    /// @return Stackable
    static StackablePtr StackableFromEntity(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
