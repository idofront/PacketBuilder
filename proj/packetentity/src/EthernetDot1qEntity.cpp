#include <EthernetDot1qEntity.hpp>

namespace PacketEntity
{
EthernetDot1qEntity::~EthernetDot1qEntity()
{
}

nlohmann::json EthernetDot1qEntity::ToJson()
{
    auto json = EthernetEntity::ToJson();
    json["VlanHeader"]["Tpid"] = this->VlanHeader.Tpid;
    json["VlanHeader"]["TagControl"]["UserPriority"] = (uint16_t)this->VlanHeader.TagControl.UserPriority;
    json["VlanHeader"]["TagControl"]["CanonicalFormat"] = (uint16_t)this->VlanHeader.TagControl.CanonicalFormat;
    json["VlanHeader"]["TagControl"]["VlanIdentifier"] = (uint16_t)this->VlanHeader.TagControl.VlanIdentifier;
    return json;
}

StackableEntityPtr EthernetDot1qEntity::FromJson(nlohmann::json json)
{
    auto entity = std::make_shared<EthernetDot1qEntity>();
    entity->DestinationMac = json["DestinationMac"];
    entity->SourceMac = json["SourceMac"];
    entity->Type = json["Type"];
    entity->VlanHeader.Tpid = json["VlanHeader"]["Tpid"];
    entity->VlanHeader.TagControl.UserPriority = json["VlanHeader"]["TagControl"]["UserPriority"];
    entity->VlanHeader.TagControl.CanonicalFormat = json["VlanHeader"]["TagControl"]["CanonicalFormat"];
    entity->VlanHeader.TagControl.VlanIdentifier = json["VlanHeader"]["TagControl"]["VlanIdentifier"];
    return entity;
}

} // namespace PacketEntity
