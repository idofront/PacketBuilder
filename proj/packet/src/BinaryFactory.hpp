#ifndef PACKET__BINARY_FACTORY_HPP
#define PACKET__BINARY_FACTORY_HPP

#include <Binary.hpp>
#include <BinaryEntity.hpp>
#include <StackableFactory.hpp>

namespace Packet
{
class BinaryFactory : public StackableFactory
{
  public:
    virtual StackablePtr Get(PacketEntity::StackableEntityPtr entity);
};
} // namespace Packet

#endif
