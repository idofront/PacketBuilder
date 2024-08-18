#include <PcapPacketHeaderEntity.hpp>
#include <Utility.hpp>

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
    json["TimestampSeconds"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "TimestampSeconds", 0);
    json["TimestampMicroseconds"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "TimestampMicroseconds", 0);
    json["IncludedLength"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "IncludedLength", 0);
    json["OriginalLength"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "OriginalLength", 0);
    return json;
}
} // namespace PacketEntity
