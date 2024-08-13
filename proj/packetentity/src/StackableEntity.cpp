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

nlohmann::json StackableEntity::ToJson()
{
    throw std::runtime_error("Not implemented");
}

StackableEntityPtr StackableEntity::Stack()
{
    return this->_Stack;
}
} // namespace PacketEntity
