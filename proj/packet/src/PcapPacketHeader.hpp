#ifndef PACKET_BUILDER__PCAP_PACKET_HEADER_HPP__
#define PACKET_BUILDER__PCAP_PACKET_HEADER_HPP__

#include <NotifyProperty.hpp>
#include <PcapPacketHeaderEntity.hpp>
#include <Stackable.hpp>
#include <chrono>
#include <pcap.h>

namespace Packet
{
struct RecordHeader
{
    uint32_t timestampSeconds;
    uint32_t timestampMicroseconds;
    uint32_t includedLength;
    uint32_t originalLength;
};

class PcapPacketHeader;
using PcapPacketHeaderPtr = std::shared_ptr<PcapPacketHeader>;

class PcapPacketHeader : public Stackable
{
  public:
    PcapPacketHeader();

    Utility::NotifyProperty<std::chrono::seconds> TimestampSeconds;
    Utility::NotifyProperty<std::chrono::microseconds> TimestampMicroseconds;
    Utility::NotifyProperty<uint32_t> IncludedLength;
    Utility::NotifyProperty<uint32_t> OriginalLength;

  protected:
    virtual void OnStacked(StackablePtr oldStackable, StackablePtr newStackable) override;

  private:
    // static const std::size_t HeaderSize = sizeof(struct pcap_pkthdr);
    static const std::size_t HeaderSize = sizeof(struct RecordHeader);
    // struct pcap_pkthdr *Header() const;
    struct RecordHeader *Header() const;

    PacketEntity::PcapPacketHeaderEntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet

#endif
