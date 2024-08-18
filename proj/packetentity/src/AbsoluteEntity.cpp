#include <AbsoluteEntity.hpp>
#include <Utility.hpp>

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

    entity->TimestampNs = Utility::ParseJsonObjectHelper<uint64_t>(json, "TimestampNs", 0);

    return entity;
}

} // namespace PacketEntity
