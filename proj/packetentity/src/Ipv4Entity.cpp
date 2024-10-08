#include <Ipv4Entity.hpp>
#include <Utility.hpp>
#include <nameof/nameof.h>
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

    try
    {
        entity->Version = Utility::ParseJsonObjectHelper<uint8_t>(json, nameof(entity->Version), 4, [](auto value) {
            if (value != 4)
            {
                // validation 機能を流用して期待しない値が入っている場合は警告する．
                auto fmt = boost::format("An unexpected version: %1%");
                auto msg = boost::str(fmt % std::uint16_t(value));
                SPDLOG_WARN(msg);
            }
            return true;
        });
        entity->IHL = Utility::ParseJsonObjectHelper<uint8_t>(json, nameof(entity->IHL), 5, [](auto value) {
            if (value != 5)
            {
                // TODO IHL が 5 以外の場合の処理を追加する．
                auto fmt = boost::format("An unsupported IHL: %1%");
                auto msg = boost::str(fmt % std::uint16_t(value));
                SPDLOG_WARN(msg);
            }
            return value == 5;
        });
        entity->DSCP = Utility::ParseJsonObjectHelper<uint8_t>(json, nameof(entity->DSCP), 0);
        entity->ECN = Utility::ParseJsonObjectHelper<uint16_t>(json, nameof(entity->ECN), 0);
        entity->TotalLength = Utility::ParseJsonObjectHelper<uint16_t>(json, nameof(entity->TotalLength), 0);
        entity->Identification = Utility::ParseJsonObjectHelper<uint16_t>(json, nameof(entity->Identification), 0);
        entity->Flags = Utility::ParseJsonObjectHelper<uint16_t>(json, nameof(entity->Flags), 0);
        entity->FragmentOffset = Utility::ParseJsonObjectHelper<uint16_t>(json, nameof(entity->FragmentOffset), 0);
        entity->TTL = Utility::ParseJsonObjectHelper<uint8_t>(json, nameof(entity->TTL), 0);
        entity->Protocol = Utility::ParseJsonObjectHelper<uint8_t>(json, nameof(entity->Protocol), 0);
        entity->HeaderChecksum = Utility::ParseJsonObjectHelper<uint16_t>(json, nameof(entity->HeaderChecksum), 0);
        entity->SourceAddress =
            Utility::ParseJsonObjectHelper<std::string>(json, nameof(entity->SourceAddress), std::nullopt);
        entity->DestinationAddress =
            Utility::ParseJsonObjectHelper<std::string>(json, nameof(entity->DestinationAddress), std::nullopt);
    }
    catch (const std::exception &e)
    {
        auto fmt = boost::format("Failed to parse json object: %1%");
        auto msg = boost::str(fmt % e.what());
        throw std::runtime_error(msg);
    }
    return entity;
}
} // namespace PacketEntity
