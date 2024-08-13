#include <UdpEntity.hpp>

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
} // namespace PacketEntity
