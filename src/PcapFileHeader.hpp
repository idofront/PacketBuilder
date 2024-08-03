#ifndef PACKET_BUILDER__PCAP_FILE_HEADER_HPP__
#define PACKET_BUILDER__PCAP_FILE_HEADER_HPP__

#include <Stackable.hpp>
#include <pcap.h>

namespace PacketBuilder
{
class PcapFileHeader;
using PcapFileHeaderPtr = std::shared_ptr<PcapFileHeader>;

class PcapFileHeader : public Stackable
{
  public:
    PcapFileHeader();

    uint32_t MagicNumber() const;
    void MagicNumber(uint32_t magicNumber);
    uint16_t VersionMajor() const;
    void VersionMajor(uint16_t versionMajor);
    uint16_t VersionMinor() const;
    void VersionMinor(uint16_t versionMinor);
    uint32_t ThisZone() const;
    void ThisZone(uint32_t thisZone);
    uint32_t SigFigs() const;
    void SigFigs(uint32_t sigFigs);
    uint32_t SnapLen() const;
    void SnapLen(uint32_t snapLen);
    uint32_t LinkType() const;
    void LinkType(uint32_t network);

  private:
    static const std::size_t HeaderSize = sizeof(struct pcap_file_header);
    struct pcap_file_header *Header() const;
};
} // namespace PacketBuilder

// file header
// c3d4 a1b2 0002 0004 0000 0000 0000 0000
// 0000 0004 0001 0000

// packet header
// ae2e 66ad // seconds
// aac5 000a // microseconds
// 0064 0000 // length
// 0064 0000 // length

// ethernet
// 0001 005e fb00 2390 675b 7e76 0008

// IP
// 0045 5600 512c 0040 1101
// a961 000a a200 00e0 fb00 e914 e914 4200
// e380 0000 0084 0000 0100 0000 0000 6118
// 616d 6f7a 2d6e 3339 3930 3035 3137 2d35
// 6941 5072 726f 0574 6f6c 6163 006c 0100
// 0180 0000 100e 0400 000a a200

// file header
// c3d4 a1b2 0002 0004 0000 0000 0000 0000
// ffff 0000 0001 0000

// packet header
// 0000 0000 // seconds
// 0000 0000 // micero seconds
// 0000 0000 // ???
// 0000 0000 // ???
// 0034 0000 // length
// 0034 0000 // length

// ethernet
// 0000 0000 0000
// 0000 0000 0000
// 0008

// ip
// 0045 2600 0000 0000 1140 b8f4 a8c0 0100 10ac
// 0100 d204 2e16 1200 0000 0000 0000 0000
// 0000 0000
//
#endif
