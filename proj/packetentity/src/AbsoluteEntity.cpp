#include <AbsoluteEntity.hpp>

namespace PacketEntity
{
AbsoluteEntity::AbsoluteEntity() : StackableEntity(), TimestampNs(0)
{
}

AbsoluteEntity::~AbsoluteEntity()
{
}

nlohmann::json AbsoluteEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["TimestampNs"] = this->TimestampNs;
    return json;
}

StackableEntityPtr AbsoluteEntity::FromJson(nlohmann::json json)
{
    auto entity = std::make_shared<AbsoluteEntity>();

    entity->TimestampNs = json["TimestampNs"];

    return entity;
}

} // namespace PacketEntity
