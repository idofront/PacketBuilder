#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <Binary.hpp>
#include <EntityService.hpp>
#include <Ethernet.hpp>
#include <Ipv4.hpp>
#include <PacketBuilder.hpp>
#include <PcapFileHeader.hpp>
#include <PcapPacketHeader.hpp>
#include <Stackable.hpp>
#include <Udp.hpp>
#include <arpa/inet.h>
#include <cmdline/cmdline.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);

    Options options;
    try
    {
        auto parser = GetArgumentParser();
        ParseArguments(options, argc, argv, parser);
        ValidateOptions(options);
    }
    catch (const std::exception &e)
    {
        SPDLOG_ERROR(e.what());
        return 1;
    }

    spdlog::set_level(options.LogLevel.Value());

    const auto isProcessingInputFile = true;
    if (isProcessingInputFile)
    {
        auto inputFilename = options.InputFilename.Value();
        auto inputStream = std::ifstream(inputFilename);
        auto inputJson = nlohmann::json::parse(inputStream);
        auto entity = PacketEntity::EntityService::FromJson(inputJson);
    }

    std::vector<Packet::StackablePtr> stackables;

    auto ether = Packet::EthernetPtr(new Packet::Ethernet());
    auto ipv4 = Packet::Ipv4Ptr(new Packet::Ipv4());
    auto udp = Packet::UdpPtr(new Packet::Udp());
    auto payload = Packet::BinaryPtr(new Packet::Binary(4));

    // Ether header
    {
        ether->DestinationMac.Value((uint8_t *)"\x00\x00\x00\x00\x00\x00");
        ether->SourceMac.Value((uint8_t *)"\x00\x00\x00\x00\x00\x00");
        ether->EthernetType.Value(ETHERTYPE_IP);
    }
    // IP header
    {
        struct sockaddr_in sourceAddress;
        struct sockaddr_in destinationAddress;
        inet_pton(AF_INET, "192.168.0.1", &(sourceAddress.sin_addr));
        inet_pton(AF_INET, "172.16.0.1", &(destinationAddress.sin_addr));
        ipv4->SourceAddress.Value(sourceAddress);
        ipv4->DestinationAddress.Value(destinationAddress);
    }

    // UDP header
    {
        udp->SourcePort(60000);
        udp->DestinationPort(50000);
    }

    std::memset(payload->DataArray().get(), 0, payload->Length());

    udp->Stack.Value(payload);
    ipv4->Stack.Value(udp);
    ether->Stack.Value(ipv4);

    for (auto i = 0; i < 1; i++)
    {
        // Compose しないと意図せず値が変わってしまうが，動作確認用に Compose せず stackables に追加する．
        // 必要に応じて Compose する．
        if (false)
        {
            auto composed = Packet::Stackable::Compose(ether);
            stackables.push_back(composed);
        }
        else
        {
            stackables.push_back(ether);
        }
    }

    if (options.OutputFileType.Value() == FileType::None)
    {
        return 0;
    }

    if (options.OutputFileType.Value() == FileType::Pcap)
    {
        SPDLOG_DEBUG("Output to pcap file");
        auto pcapFileHeader = Packet::PcapFileHeaderPtr(new Packet::PcapFileHeader());
        SPDLOG_DEBUG("File Header: \n{}", Packet::Stackable::HexDump(pcapFileHeader));

        std::for_each(stackables.begin(), stackables.end(), [&pcapFileHeader](Packet::StackablePtr stackable) {
            auto pcapPacketHeader = Packet::PcapPacketHeaderPtr(new Packet::PcapPacketHeader());
            pcapPacketHeader->Stack.Value(stackable);

            auto tail = Packet::Stackable::Tail(pcapFileHeader);
            tail->Stack.Value(pcapPacketHeader);
            SPDLOG_DEBUG("Packet Header: \n{}", Packet::Stackable::HexDump(pcapPacketHeader));
        });

        auto json = pcapFileHeader->StackableEntity()->ToJson();
        SPDLOG_DEBUG("\n{}", json.dump(4));

        std::ofstream fs(options.OutputFilename.Value(), std::ios::out | std::ios::binary);
        auto composed = Packet::Stackable::Compose(pcapFileHeader);

        auto dump = Packet::Stackable::HexDump(composed);
        SPDLOG_TRACE("Dump: \n{}", dump);

        auto ptr = (const char *)composed->DataArray().get();
        fs.write(ptr, composed->Length());
        fs.close();
    }

    return 0;
}
