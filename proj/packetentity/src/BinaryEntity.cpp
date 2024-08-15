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
    auto entity = std::make_shared<BinaryEntity>(8);

    SPDLOG_WARN("BinaryEntity::FromJson not implemented");

    return entity;
}

} // namespace PacketEntity
