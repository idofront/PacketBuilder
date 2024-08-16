#include <BinaryEntity.hpp>
#include <spdlog/spdlog.h>

namespace PacketEntity
{
BinaryEntity::BinaryEntity(std::size_t length) : StackableEntity(), Data(std::make_shared<DataArray>(length))
{
}

BinaryEntity::~BinaryEntity()
{
}

nlohmann::json BinaryEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["Data"] = *(this->Data);
    return json;
}

StackableEntityPtr BinaryEntity::FromJson(nlohmann::json json)
{
    auto length = json["Data"].size();

    auto entity = std::make_shared<BinaryEntity>(length);

    for (std::size_t i = 0; i < length; i++)
    {
        entity->Data->at(i) = json["Data"][i];
    }

    return entity;
}

} // namespace PacketEntity
