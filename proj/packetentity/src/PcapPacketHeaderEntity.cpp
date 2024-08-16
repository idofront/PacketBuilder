#include <PcapPacketHeaderEntity.hpp>

namespace PacketEntity
{
PcapPacketHeaderEntity::PcapPacketHeaderEntity() : StackableEntity()
{
}

PcapPacketHeaderEntity::~PcapPacketHeaderEntity()
{
}

nlohmann::json PcapPacketHeaderEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["TimestampSeconds"] = this->Seconds;
    json["TimestampMicroseconds"] = this->Microseconds;
    json["IncludedLength"] = this->IncludedLength;
    json["OriginalLength"] = this->OriginalLength;
    return json;
}
} // namespace PacketEntity
