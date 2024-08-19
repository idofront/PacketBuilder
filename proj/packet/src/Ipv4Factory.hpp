#ifndef PACKET__IPV4_FACTORY_HPP
#define PACKET__IPV4_FACTORY_HPP

#include <Ipv4.hpp>
#include <Ipv4Entity.hpp>
#include <StackableFactory.hpp>

namespace Packet
{
class Ipv4Factory : public StackableFactory
{
  public:
    virtual StackablePtr Get(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
