#include <Ipv4Entity.hpp>
#include <spdlog/spdlog.h>

namespace PacketEntity
{
Ipv4Entity::Ipv4Entity() : StackableEntity()
{
}

Ipv4Entity::~Ipv4Entity()
{
}

nlohmann::json Ipv4Entity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["Version"] = this->Version;
    json["IHL"] = this->IHL;
    json["DSCP"] = this->DSCP;
    json["ECN"] = this->ECN;
    json["TotalLength"] = this->TotalLength;
    json["Identification"] = this->Identification;
    json["Flags"] = this->Flags;
    json["FragmentOffset"] = this->FragmentOffset;
    json["TTL"] = this->TTL;
    json["Protocol"] = this->Protocol;
    json["HeaderChecksum"] = this->HeaderChecksum;
    json["SourceAddress"] = this->SourceAddress;
    json["DestinationAddress"] = this->DestinationAddress;
    return json;
}

StackableEntityPtr Ipv4Entity::FromJson(nlohmann::json json)
{
    auto entity = std::make_shared<Ipv4Entity>();

    entity->Version = json["Version"].get_to(entity->Version);
    entity->IHL = json["IHL"].get_to(entity->IHL);
    entity->DSCP = json["DSCP"].get_to(entity->DSCP);
    entity->ECN = json["ECN"].get_to(entity->ECN);
    entity->TotalLength = json["TotalLength"].get_to(entity->TotalLength);
    entity->Identification = json["Identification"].get_to(entity->Identification);
    entity->Flags = json["Flags"].get_to(entity->Flags);
    entity->FragmentOffset = json["FragmentOffset"].get_to(entity->FragmentOffset);
    entity->TTL = json["TTL"].get_to(entity->TTL);
    entity->Protocol = json["Protocol"].get_to(entity->Protocol);
    entity->HeaderChecksum = json["HeaderChecksum"].get_to(entity->HeaderChecksum);
    entity->SourceAddress = json["SourceAddress"].get_to(entity->SourceAddress);
    entity->DestinationAddress = json["DestinationAddress"].get_to(entity->DestinationAddress);

    return entity;
}
} // namespace PacketEntity
