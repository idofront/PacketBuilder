#ifndef PACKET__UDP_FACTORY_HPP
#define PACKET__UDP_FACTORY_HPP

#include <StackableFactory.hpp>
#include <Udp.hpp>
#include <UdpEntity.hpp>

namespace Packet
{
class UdpFactory : public StackableFactory
{
  public:
    virtual StackablePtr Get(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
