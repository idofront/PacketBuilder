#ifndef PACKET_ENTITY__PCAP_FILE_HEADER_ENTITY_HPP
#define PACKET_ENTITY__PCAP_FILE_HEADER_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class PcapFileHeaderEntity;
using PcapFileHeaderEntityPtr = std::shared_ptr<PcapFileHeaderEntity>;
class PcapFileHeaderEntity : public StackableEntity
{
  public:
    PcapFileHeaderEntity();
    virtual ~PcapFileHeaderEntity();
    virtual nlohmann::json ToJson() override;

  public:
    std::uint32_t MagicNumber;
    std::uint16_t MajorVersion;
    std::uint16_t MinorVersion;
    std::uint32_t ThisZone;
    std::uint32_t SigFigs;
    std::uint32_t SnapLen;
    std::uint32_t Network;
};
} // namespace PacketEntity
#endif
