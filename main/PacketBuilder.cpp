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
#include <boost/format.hpp>
#include <cmdline/cmdline.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <pcap.h>
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

    auto actions = std::map<FileType, std::function<void(void)>>{
        {FileType::Pcap,
         [&]() {
             auto fmt = boost::format("Output to pcap file: %1%");
             auto msg = fmt % options.OutputFilename.Value();
             SPDLOG_INFO(msg.str());

             SaveAsPcap(options.OutputFilename.Value(), stackables);
         }},
    };
    auto actionItr = actions.find(options.OutputFileType.Value());
    if (actionItr == actions.end())
    {
        SPDLOG_ERROR("Invalid output file type");
        return 1;
    }
    actionItr->second();

    return 0;
}
