#include <UdpEntity.hpp>
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

    SPDLOG_WARN("UdpEntity::FromJson not implemented");

    return entity;
}
} // namespace PacketEntity
