#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <Binary.hpp>
#include <EntityService.hpp>
#include <Ethernet.hpp>
#include <Ipv4.hpp>
#include <PacketBuilder.hpp>
#include <PacketService.hpp>
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

    auto inputFilename = options.InputFilename.Value();
    auto inputStream = std::ifstream(inputFilename);
    auto inputJson = nlohmann::json::parse(inputStream);
    auto entities = PacketEntity::EntityService::ParseEntities(inputJson);

    std::vector<Packet::StackablePtr> stackables;
    std::transform(
        entities.begin(), entities.end(), std::back_inserter(stackables),
        [](PacketEntity::StackableEntityPtr entity) { return Packet::PacketService::StackableFromEntity(entity); });

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
            if (stackable == nullptr)
            {
                throw std::runtime_error("The stackable is null.");
            }

            // Stackable が Absolute であることを検証する
            auto absolute = std::dynamic_pointer_cast<Packet::Absolute>(stackable);
            if (absolute == nullptr)
            {
                throw std::runtime_error("The stackable is not Absolute.");
            }

            // Absolute を PcapPacketHeader に変換する
            auto pcapPacketHeader = CreatePcapPacketHeader(absolute);

            // Absolute がもつ Stackable を取得する
            auto stackableEntity = absolute->Stack.Value();
            if (stackableEntity == nullptr)
            {
                throw std::runtime_error("The stackable entity is null.");
            }

            pcapPacketHeader->Stack.Value(stackableEntity);

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
