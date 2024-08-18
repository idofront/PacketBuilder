#include <PcapFileHeaderEntity.hpp>
#include <Utility.hpp>

namespace PacketEntity
{
PcapFileHeaderEntity::PcapFileHeaderEntity() : StackableEntity()
{
}

PcapFileHeaderEntity::~PcapFileHeaderEntity()
{
}

nlohmann::json PcapFileHeaderEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["MagicNumber"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "MagicNumber", 0);
    json["MajorVersion"] = Utility::ParseJsonObjectHelper<uint16_t>(json, "MajorVersion", 0);
    json["MinorVersion"] = Utility::ParseJsonObjectHelper<uint16_t>(json, "MinorVersion", 0);
    json["ThisZone"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "ThisZone", 0);
    json["SigFigs"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "SigFigs", 0);
    json["SnapLen"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "SnapLen", 0);
    json["Network"] = Utility::ParseJsonObjectHelper<uint32_t>(json, "Network", 0);
    return json;
}
} // namespace PacketEntity
