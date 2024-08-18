#include <UdpEntity.hpp>
#include <Utility.hpp>
#include <spdlog/spdlog.h>

namespace PacketEntity
{
UdpEntity::UdpEntity() : PacketEntity::StackableEntity()
{
}

UdpEntity::~UdpEntity()
{
}

nlohmann::json UdpEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["SourcePort"] = this->SourcePort;
    json["DestinationPort"] = this->DestinationPort;
    json["Length"] = this->Length;
    json["Checksum"] = this->Checksum;
    return json;
}

StackableEntityPtr UdpEntity::FromJson(nlohmann::json json)
{
    auto entity = std::make_shared<UdpEntity>();

    entity->SourcePort = Utility::ParseJsonObjectHelper<uint16_t>(json, "SourcePort", 0);
    entity->DestinationPort = Utility::ParseJsonObjectHelper<uint16_t>(json, "DestinationPort", 0);
    entity->Length = Utility::ParseJsonObjectHelper<uint16_t>(json, "Length", 0);
    entity->Checksum = Utility::ParseJsonObjectHelper<uint16_t>(json, "Checksum", 0);

    return entity;
}
} // namespace PacketEntity
