#ifndef PACKET__ABSOLUTE_FACTORY_HPP
#define PACKET__ABSOLUTE_FACTORY_HPP

#include <Absolute.hpp>
#include <AbsoluteEntity.hpp>
#include <StackableFactory.hpp>

namespace Packet
{
class AbsoluteFactory : public StackableFactory
{
  public:
    virtual StackablePtr Get(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
