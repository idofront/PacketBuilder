#include <BinaryEntity.hpp>

namespace PacketEntity
{
BinaryEntity::BinaryEntity() : StackableEntity()
{
}

BinaryEntity::~BinaryEntity()
{
}

nlohmann::json BinaryEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["Data"] = nlohmann::json::parse(this->Data->begin(), this->Data->end());
    return json;
}

} // namespace PacketEntity
