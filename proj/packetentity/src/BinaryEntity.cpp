#include <BinaryEntity.hpp>
#include <Utility.hpp>
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
    auto array = Utility::ParseJsonObjectHelper<DataArray>(json, "Data", DataArray());
    auto length = array.size();

    auto entity = std::make_shared<BinaryEntity>(length);

    for (std::size_t i = 0; i < length; i++)
    {
        entity->Data->at(i) = json["Data"][i];
    }

    return entity;
}

std::string BinaryEntity::ToString()
{
    throw std::runtime_error("Not implemented");
}

} // namespace PacketEntity
