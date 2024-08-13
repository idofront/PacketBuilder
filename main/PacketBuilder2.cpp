#include <Converter.hpp>
#include <EthernetHeader.hpp>
#include <Ipv4.hpp>
#include <JCEthernetHeaderEntity.hpp>
#include <JCIpv4Entity.hpp>
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

    auto ipv4 = std::make_shared<PacketBuilder::Ipv4>();
    {
        ipv4->Version(4);
        ipv4->Ihl(5);
        ipv4->Tos(0);
        ipv4->TotalLength(20);
        ipv4->Id(0);
        ipv4->Flags(0);
        ipv4->Ttl(64);
        ipv4->Protocol(17);
        ipv4->Checksum(0);
        auto sourceIp = (uint32_t)0x0A0B0C0D;
        ipv4->SourceIp(sourceIp);
        auto destinationIp = (uint32_t)0x0E0F1011;
        ipv4->DestinationIp(destinationIp);
    }
    auto ipv4Entity = JsonConverter::Ipv4Entity();

    auto udp = std::make_shared<PacketBuilder::Udp>();
    {
        udp->SourcePort(12345);
        udp->DestinationPort(54321);
        udp->UdpLength(8);
        udp->UdpChecksum(0);
    }
    auto udpEntity = JsonConverter::UdpEntity();

    ethernetHeader->Stack(ipv4);
    ipv4->Stack(udp);

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

    JsonConverter::StackableEntityPtr entityPtr =
        std::make_shared<JsonConverter::EthernetHeaderEntity>(ethernetHeaderEntity);
    while (entityPtr)
    {
        auto str = entityPtr->ToString();
        spdlog::debug("{}", str);
        entityPtr = entityPtr->Stack;
    }
}

/// @brief 動作確認用仮実装
int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);

    auto json = ToJson();
    FromJson(json);
    return 0;
}
