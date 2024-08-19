#ifndef PACKET__ETHERNET_FACTORY_HPP
#define PACKET__ETHERNET_FACTORY_HPP

#include <Ethernet.hpp>
#include <StackableFactory.hpp>

namespace Packet
{
class EthernetFactory : public StackableFactory
{
  public:
    virtual StackablePtr Get(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
