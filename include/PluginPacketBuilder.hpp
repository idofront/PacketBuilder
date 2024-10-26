#ifndef PACKET_BUILDER__PLUGIN_PACKET_BUILDER_HPP
#define PACKET_BUILDER__PLUGIN_PACKET_BUILDER_HPP

#include <AbsoluteEntity.hpp>
#include <EntityService.hpp>
#include <Packet/Stackable.hpp>
#include <PcapFileHeaderEntity.hpp>
#include <PcapPacketHeaderEntity.hpp>
#include <PluginContainer.hpp>
#include <ReadOnlyObservableProperty.hpp>
#include <filesystem>
#include <memory>
#include <pcap.h>
#include <string.h>

namespace PluginPacketBuilder
{
class PluginPacketBuilder
{
  public:
    PluginPacketBuilder(PluginContract::PluginContainerPtr container) : _Container(container)
    {
    }
    virtual ~PluginPacketBuilder() = default;

    void ParsePcap(const std::filesystem::path &path)
    {
        this->ParsePcap(path);
    }

  private:
    using StackableEntityPtrs = std::vector<PacketEntity::StackableEntityPtr>;
    PluginContract::PluginContainerPtr _Container;
    void ReadPcap(const std::filesystem::path &path)
    {
        pcap_t *pcap = pcap_open_offline(path.c_str(), nullptr);
        if (pcap == nullptr)
        {
            throw std::runtime_error("Failed to open pcap file.");
        }

        // パケットデータを解析して Entity に変換
        auto stackableEntityPtrs = ParsePcapHeader(pcap);

        // 解析処理後の Entity を入れるための配列
        auto parsedEntities = std::vector<PacketEntity::StackableEntityPtr>();

        std::transform(stackableEntityPtrs.begin(), stackableEntityPtrs.end(), std::back_inserter(parsedEntities),
                       [&](const auto &entityPtr) { return ParsePcapData(entityPtr); });
        pcap_close(pcap);
    }

    StackableEntityPtrs ParsePcapHeader(pcap_t *pcap)
    {
        auto stackableEntities = std::vector<PacketEntity::StackableEntityPtr>();
        while (true)
        {
            struct pcap_pkthdr *header;
            const uint8_t *packet;
            int result = pcap_next_ex(pcap, &header, &packet);

            if (result == 1)
            {
                auto pcapPacketHeaderEntity = std::make_shared<PacketEntity::PcapPacketHeaderEntity>();
                auto timestamp = header->ts;
                pcapPacketHeaderEntity->Seconds = timestamp.tv_sec;
                pcapPacketHeaderEntity->Microseconds = timestamp.tv_usec;
                pcapPacketHeaderEntity->IncludedLength = header->caplen;
                pcapPacketHeaderEntity->OriginalLength = header->len;

                auto length = header->caplen;
                auto binaryEntityPtr = std::make_shared<PacketEntity::BinaryEntity>(length);

                std::memcpy(binaryEntityPtr->Data->data(), packet, length);

                pcapPacketHeaderEntity->Stack.Value(binaryEntityPtr);

                stackableEntities.push_back(pcapPacketHeaderEntity);
            }
            else if (result == 0)
            {
                auto fmt = boost::format("Timeout occurred.");
                auto msg = fmt.str();
                SPDLOG_INFO(msg);
                continue;
            }
            else if (result == -2)
            {
                auto fmt = boost::format("End of file.");
                auto msg = fmt.str();
                SPDLOG_INFO(msg);
                break;
            }
            else
            {
                throw std::runtime_error("Failed to read pcap file.");
            }
        }

        return stackableEntities;
    }

    PacketEntity::StackableEntityPtr ParsePcapData(const PacketEntity::StackableEntityPtr stackableEntityPtr)
    {
        // TODO StackableEntity でなく Stackable を引数にする必要がる
        auto stackable = (PluginContract::Packet::StackablePtr) nullptr;

        while (true)
        {
            auto parsers = _Container->FilterParsers(stackable);

            if (parsers.size() == 0)
            {
                auto format = boost::format("No parser found");
                auto message = format.str();
                SPDLOG_INFO(message);
                return stackableEntityPtr;
            }

            if (parsers.size() > 1)
            {
                auto format = boost::format("Multiple parsers found. Use the first one.");
                auto message = format.str();
                SPDLOG_INFO(message);
            }

            auto parser = parsers[0];

            parser->Parse(stackable);
        }

        auto pcapPacketHeaderEntity = std::make_shared<PacketEntity::PcapPacketHeaderEntity>();
        auto binaryEntityPtr = std::make_shared<PacketEntity::BinaryEntity>(0);
        pcapPacketHeaderEntity->Stack.Value(binaryEntityPtr);
        return pcapPacketHeaderEntity;
    }
};
} // namespace PluginPacketBuilder

#endif
