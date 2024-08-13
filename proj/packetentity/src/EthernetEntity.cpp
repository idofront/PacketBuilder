#include <EthernetEntity.hpp>

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
} // namespace PacketEntity
