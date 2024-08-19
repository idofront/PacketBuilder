#ifndef PACKET__STACKABLE_FACTORY_HPP
#define PACKET__STACKABLE_FACTORY_HPP

#include <Stackable.hpp>

namespace Packet
{
class StackableFactory;
using StackableFactoryPtr = std::shared_ptr<StackableFactory>;
class StackableFactory
{
  public:
    virtual StackablePtr Get(PacketEntity::StackableEntityPtr entity) = 0;
};
} // namespace Packet

#endif
