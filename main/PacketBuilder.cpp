#include <Ethernet.hpp>
#include <Ipv4.hpp>
#include <PacketBuilder.hpp>
#include <PcapFileHeader.hpp>
#include <PcapPacketHeader.hpp>
#include <Stackable.hpp>
#include <Udp.hpp>
#include <Utility/Utility.hpp>
#include <arpa/inet.h>
#include <cmdline.h>
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

    spdlog::set_level(options.LogLevel());

    std::vector<PacketBuilder::StackablePtr> stackables;

    auto ether = PacketBuilder::EthernetPtr(new PacketBuilder::Ethernet());
    auto ipv4 = PacketBuilder::Ipv4Ptr(new PacketBuilder::Ipv4());
    auto udp = PacketBuilder::UdpPtr(new PacketBuilder::Udp());
    auto payload = PacketBuilder::StackablePtr(new PacketBuilder::Stackable(32));

    // Ether header
    {
        ether->DestinationMac((uint8_t *)"\x00\x00\x00\x00\x00\x00");
        ether->SourceMac((uint8_t *)"\x00\x00\x00\x00\x00\x00");
        ether->EthernetType(ETHERTYPE_IP);
    }
    // IP header
    {
        struct sockaddr_in sourceAddress;
        struct sockaddr_in destinationAddress;
        inet_pton(AF_INET, "192.168.0.1", &(sourceAddress.sin_addr));
        inet_pton(AF_INET, "172.16.0.1", &(destinationAddress.sin_addr));
        ipv4->SourceIp(sourceAddress);
        ipv4->DestinationIp(destinationAddress);
    }

    // UDP header
    {
        udp->SourcePort(60000);
        udp->DestinationPort(40000);
    }

    std::memset(payload->DataArray().get(), 0, 10);

    udp->Stack(payload);
    ipv4->Stack(udp);
    ether->Stack(ipv4);

    for (auto i = 0; i < 5; i++)
    {
        auto composed = PacketBuilder::Stackable::Compose(ether);
        stackables.push_back(composed);
    }

    if (options.OutputFileType() == FileType::None)
    {
        return 0;
    }

    if (options.OutputFileType() == FileType::Pcap)
    {
        SPDLOG_DEBUG("Output to pcap file");
        auto pcapFileHeader = PacketBuilder::PcapFileHeaderPtr(new PacketBuilder::PcapFileHeader());
        SPDLOG_DEBUG("File Header: \n{}", PacketBuilder::Stackable::HexDump(pcapFileHeader));

        std::for_each(stackables.begin(), stackables.end(), [&pcapFileHeader](PacketBuilder::StackablePtr stackable) {
            auto pcapPacketHeader = PacketBuilder::PcapPacketHeaderPtr(new PacketBuilder::PcapPacketHeader());
            pcapPacketHeader->Stack(stackable);

            auto tail = PacketBuilder::Stackable::Tail(pcapFileHeader);
            tail->Stack(pcapPacketHeader);
            SPDLOG_DEBUG("Packet Header: \n{}", PacketBuilder::Stackable::HexDump(pcapPacketHeader));
        });

        std::ofstream fs(options.OutputFilename(), std::ios::out | std::ios::binary);
        auto composed = PacketBuilder::Stackable::Compose(pcapFileHeader);

        auto dump = PacketBuilder::Stackable::HexDump(composed);
        SPDLOG_TRACE("Dump: \n{}", dump);

        auto ptr = (const char *)composed->DataArray().get();
        fs.write(ptr, composed->Length());
        fs.close();
    }

    return 0;
}
