#include <EthernetEntity.hpp>
#include <Utility.hpp>
#include <net/ethernet.h>

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
    entity->DestinationMac = Utility::ParseJsonObjectHelper<std::string>(json, "DestinationMac", "");
    entity->SourceMac = Utility::ParseJsonObjectHelper<std::string>(json, "SourceMac", "");
    entity->Type = Utility::ParseJsonObjectHelper<uint16_t>(json, "Type", ETHERTYPE_IP);
    return entity;
}
} // namespace PacketEntity
