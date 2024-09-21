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
#include <iostream>
#include <pcap.h>
#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    auto errorMessage = std::optional<std::string>();

    try
    {
        spdlog::set_level(spdlog::level::trace);

        auto options = Options();
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

        // 入力ファイルに対する処理を定義
        auto input_actions = std::map<FileType, std::function<std::vector<Packet::StackablePtr>(void)>>{
            {FileType::Pcap,
             [&]() -> std::vector<Packet::StackablePtr> {
                 auto fmt = boost::format("Input from pcap file: %1%");
                 auto msg = fmt % options.InputFilename.Value();
                 SPDLOG_INFO(msg.str());

                 auto inputFilename = options.InputFilename.Value();
                 auto entities = PacketEntity::EntityService::ParsePcap(inputFilename);

                 auto stackables = std::vector<Packet::StackablePtr>();
                 std::transform(entities.begin(), entities.end(), std::back_inserter(stackables),
                                [](PacketEntity::StackableEntityPtr entity) -> Packet::StackablePtr {
                                    try
                                    {
                                        return Packet::PacketService::StackableFromEntity(entity);
                                    }
                                    catch (const std::exception &e)
                                    {
                                        SPDLOG_ERROR(e.what());
                                        return nullptr;
                                    }
                                });
                 return stackables;
             }},
            {FileType::Json,
             [&]() {
                 auto fmt = boost::format("Input from json file: %1%");
                 auto msg = fmt % options.InputFilename.Value();
                 SPDLOG_INFO(msg.str());

                 auto inputFilename = options.InputFilename.Value();
                 auto entities = PacketEntity::EntityService::ParseEntities(inputFilename);

                 auto stackables = std::vector<Packet::StackablePtr>();
                 std::transform(entities.begin(), entities.end(), std::back_inserter(stackables),
                                [](PacketEntity::StackableEntityPtr entity) -> Packet::StackablePtr {
                                    try
                                    {
                                        return Packet::PacketService::StackableFromEntity(entity);
                                    }
                                    catch (const std::exception &e)
                                    {
                                        SPDLOG_ERROR(e.what());
                                        return nullptr;
                                    }
                                });
                 return stackables;
             }},
        };
        auto inputActionItr = input_actions.find(options.InputFileType.Value());
        if (inputActionItr == input_actions.end())
        {
            SPDLOG_ERROR("Invalid input file type");
            return 1;
        }
        auto stackables = inputActionItr->second();

        // 出力ファイルに対する処理を定義
        auto output_actions = std::map<FileType, std::function<void(void)>>{
            {FileType::Pcap,
             [&]() {
                 auto fmt = boost::format("Output to pcap file: %1%");
                 auto msg = fmt % options.OutputFilename.Value();
                 SPDLOG_INFO(msg.str());

                 SaveAsPcap(options.OutputFilename.Value(), stackables);
             }},
            {FileType::Json,
             [&]() {
                 auto fmt = boost::format("Output to json file: %1%");
                 auto msg = fmt % options.OutputFilename.Value();
                 SPDLOG_INFO(msg.str());

                 // SaveAsJson(options.OutputFilename.Value(), stackables);
                 throw std::runtime_error("Not implemented");
             }},
            {FileType::None,
             [&]() {
                 auto fmt = boost::format("Output is disabled");
                 auto msg = fmt;
                 SPDLOG_INFO(msg.str());

                 for (auto num = 0; num < stackables.size(); num++)
                 {
                     SPDLOG_INFO("Packet #{}", num);
                     auto stackable = stackables[num];

                     // 出力
                     while (stackable != nullptr)
                     {
                         auto type_name = Utility::Demangle(typeid(*stackable).name());
                         auto fmt = boost::format("%1%:\n%2%");
                         auto msg = fmt % type_name % Packet::Stackable::HexDump(stackable);
                         SPDLOG_INFO(msg.str());

                         stackable = stackable->Stack.Value();
                     }
                 }
             }},
        };
        auto outputActionItr = output_actions.find(options.OutputFileType.Value());
        if (outputActionItr == output_actions.end())
        {
            SPDLOG_ERROR("Invalid output file type");
            return 1;
        }
        outputActionItr->second();
    }
    catch (const std::exception &e)
    {
        auto fmt = boost::format("Error: %1%");
        auto msg = fmt % e.what();

        errorMessage = msg.str();
    }
    catch (...)
    {
        auto fmt = boost::format("Critical error");
        auto msg = fmt;

        errorMessage = msg.str();
    }

    if (errorMessage.has_value())
    {
        SPDLOG_ERROR(errorMessage.value());
        return 1;
    }

    return 0;
}
