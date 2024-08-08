#include <Converter.hpp>
#include <EthernetHeader.hpp>
#include <EthernetHeaderEntity.hpp>
#include <spdlog/spdlog.h>

std::string ToJson()
{
    auto ethernetHeader = std::make_shared<PacketBuilder::EthernetHeader>();
    {
        ethernetHeader->DestinationMac((uint8_t *)"\x0A\x0B\x0C\x0D\x0E\x0F");
        ethernetHeader->SourceMac((uint8_t *)"\x01\x02\x03\x04\x05\x06");
        ethernetHeader->EthernetType(ETHERTYPE_IP);
    }
    auto ethernetHeaderEntity = JsonConverter::EthernetHeaderEntity();

    JsonConverter::to_entity(ethernetHeaderEntity, *ethernetHeader);

    auto ethernetHeaderJson = nlohmann::json(ethernetHeaderEntity);

    auto jsonString = ethernetHeaderJson.dump(4);
    SPDLOG_DEBUG("{}", jsonString);
    return jsonString;
}

void FromJson(std::string jsonString)
{
    auto ethernetHeaderEntity = JsonConverter::EthernetHeaderEntity();
    auto ethernetHeader = std::make_shared<PacketBuilder::EthernetHeader>();

    auto ethernetHeaderJson = nlohmann::json::parse(jsonString);

    ethernetHeaderEntity = ethernetHeaderJson.get<JsonConverter::EthernetHeaderEntity>();

    JsonConverter::from_entity(ethernetHeaderEntity, *ethernetHeader);

    SPDLOG_DEBUG("DestinationMac: {}", ethernetHeaderEntity.DestinationMac);
    SPDLOG_DEBUG("SourceMac: {}", ethernetHeaderEntity.SourceMac);
    SPDLOG_DEBUG("EthernetType: 0x{:04X}", ethernetHeaderEntity.EthernetType);
    SPDLOG_DEBUG("StackType: {}", ethernetHeaderEntity.StackType);
}

/// @brief 動作確認用仮実装
int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);

    auto json = ToJson();
    FromJson(json);
    return 0;
}
