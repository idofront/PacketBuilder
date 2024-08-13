#include <StackableEntity.hpp>

namespace PacketEntity
{
StackableEntity::StackableEntity()
{
}

StackableEntityPtr StackableEntity::Stack()
{
    return this->_Stack;
}
} // namespace PacketEntity
