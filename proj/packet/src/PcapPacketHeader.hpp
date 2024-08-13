#ifndef PACKET_BUILDER__PCAP_PACKET_HEADER_HPP__
#define PACKET_BUILDER__PCAP_PACKET_HEADER_HPP__

#include <Stackable.hpp>
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

    uint32_t TimestampSeconds() const;
    void TimestampSeconds(uint32_t timestampSeconds);
    uint32_t TimestampMicroseconds() const;
    void TimestampMicroseconds(uint32_t timestampMicroseconds);
    uint32_t IncludedLength() const;
    void IncludedLength(uint32_t includedLength);
    uint32_t OriginalLength() const;
    void OriginalLength(uint32_t originalLength);

  protected:
    virtual void OnStacked() override;

  private:
    // static const std::size_t HeaderSize = sizeof(struct pcap_pkthdr);
    static const std::size_t HeaderSize = sizeof(struct RecordHeader);
    // struct pcap_pkthdr *Header() const;
    struct RecordHeader *Header() const;
};
} // namespace Packet

#endif
