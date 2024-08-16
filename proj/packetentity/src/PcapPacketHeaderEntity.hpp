#ifndef PACKET_ENTITY__PCAP_PACKET_HEADER_ENTITY_HPP
#define PACKET_ENTITY__PCAP_PACKET_HEADER_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class PcapPacketHeaderEntity;
using PcapPacketHeaderEntityPtr = std::shared_ptr<PcapPacketHeaderEntity>;
class PcapPacketHeaderEntity : public StackableEntity
{
  public:
    PcapPacketHeaderEntity();
    virtual ~PcapPacketHeaderEntity();
    virtual nlohmann::json ToJson() override;

  public:
    std::uint32_t Seconds;
    std::uint32_t Microseconds;
    std::uint32_t IncludedLength;
    std::uint32_t OriginalLength;
};
} // namespace PacketEntity
#endif
