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

    SPDLOG_WARN("Ipv4Entity::FromJson not implemented");

    return entity;
}
} // namespace PacketEntity
