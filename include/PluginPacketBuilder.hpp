#ifndef PACKET_BUILDER__PLUGIN_PACKET_BUILDER_HPP
#define PACKET_BUILDER__PLUGIN_PACKET_BUILDER_HPP

#include <AbsoluteEntity.hpp>
#include <EntityService.hpp>
#include <PcapFileHeaderEntity.hpp>
#include <PcapPacketHeaderEntity.hpp>
#include <PluginContainer.hpp>
#include <filesystem>
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
    class PcapDataSet
    {
      public:
        PcapDataSet(std::vector<PacketEntity::StackableEntityPtr> stackableEntities)
            : _StackableEntities(stackableEntities)
        {
        }

      private:
        std::vector<PacketEntity::StackableEntityPtr> _StackableEntities;
    };
    using PcapDataSetPtr = std::shared_ptr<PcapDataSet>;

    PluginContract::PluginContainerPtr _Container;
    void ReadPcap(const std::filesystem::path &path)
    {
        pcap_t *pcap = pcap_open_offline(path.c_str(), nullptr);
        if (pcap == nullptr)
        {
            throw std::runtime_error("Failed to open pcap file.");
        }

        ParsePcapHeader(pcap);
        ParsePcapData(pcap);
        pcap_close(pcap);
    }

    PcapDataSetPtr ParsePcapHeader(pcap_t *pcap)
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

        return std::make_shared<PcapDataSet>(stackableEntities);
    }
    void ParsePcapData(pcap_t *pcap)
    {
    }
};
} // namespace PluginPacketBuilder

#endif
