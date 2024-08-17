#ifndef PACKET_BUILDER__PCAP_FILE_HEADER_HPP__
#define PACKET_BUILDER__PCAP_FILE_HEADER_HPP__

#include <Stackable.hpp>
#include <pcap.h>

namespace Packet
{
class PcapFileHeader;
using PcapFileHeaderPtr = std::shared_ptr<PcapFileHeader>;
/// @brief Pcapファイルヘッダを表現する．
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
} // namespace Packet

#endif
