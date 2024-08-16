#include <PcapFileHeaderEntity.hpp>

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
    json["MagicNumber"] = this->MagicNumber;
    json["MajorVersion"] = this->MajorVersion;
    json["MinorVersion"] = this->MinorVersion;
    json["ThisZone"] = this->ThisZone;
    json["SigFigs"] = this->SigFigs;
    json["SnapLen"] = this->SnapLen;
    json["Network"] = this->Network;
    return json;
}
} // namespace PacketEntity
