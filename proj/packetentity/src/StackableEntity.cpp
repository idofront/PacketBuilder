#include <StackableEntity.hpp>

namespace PacketEntity
{
StackableEntity::StackableEntity()
{
}

StackableEntity::~StackableEntity()
{
}

std::string StackableEntity::ToString()
{
    auto json = this->ToJson();
    return json.dump();
}

StackableEntityPtr StackableEntity::Stack()
{
    return this->_Stack;
}
} // namespace PacketEntity
