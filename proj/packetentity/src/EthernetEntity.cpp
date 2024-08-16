#include <EthernetEntity.hpp>
#include <spdlog/spdlog.h>

namespace PacketEntity
{
EthernetEntity::EthernetEntity() : PacketEntity::StackableEntity()
{
}

EthernetEntity::~EthernetEntity()
{
}

nlohmann::json EthernetEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["DestinationMac"] = this->DestinationMac;
    json["SourceMac"] = this->SourceMac;
    json["Type"] = this->Type;
    return json;
}

StackableEntityPtr EthernetEntity::FromJson(nlohmann::json json)
{
    auto entity = std::make_shared<EthernetEntity>();
    entity->DestinationMac = json["DestinationMac"];
    entity->SourceMac = json["SourceMac"];
    entity->Type = json["Type"];
    return entity;
}
} // namespace PacketEntity
