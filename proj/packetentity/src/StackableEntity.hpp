#ifndef PACKET_ENTITY__STACKABLE_ENTITY_HPP
#define PACKET_ENTITY__STACKABLE_ENTITY_HPP

#include <memory>

namespace PacketEntity
{
class StackableEntity;
using StackableEntityPtr = std::shared_ptr<StackableEntity>;

class StackableEntity
{
  public:
    StackableEntity();
    StackableEntityPtr Stack();
  private:
    StackableEntityPtr _Stack;
};
} // namespace PacketEntity

#endif
