#include <PcapFileHeader.hpp>

namespace Packet
{
PcapFileHeader::PcapFileHeader() : Stackable(HeaderSize)
{
    this->MagicNumber(0xa1b2c3d4);
    this->VersionMajor(2);
    this->VersionMinor(4);
    this->ThisZone(0);
    this->SigFigs(0);
    this->SnapLen(65535);
    this->LinkType(1);
}

uint32_t PcapFileHeader::MagicNumber() const
{
    return this->Header()->magic;
}

void PcapFileHeader::MagicNumber(uint32_t magicNumber)
{
    this->Header()->magic = magicNumber;
}

uint16_t PcapFileHeader::VersionMajor() const
{
    return this->Header()->version_major;
}

void PcapFileHeader::VersionMajor(uint16_t versionMajor)
{
    this->Header()->version_major = versionMajor;
}

uint16_t PcapFileHeader::VersionMinor() const
{
    return this->Header()->version_minor;
}

void PcapFileHeader::VersionMinor(uint16_t versionMinor)
{
    this->Header()->version_minor = versionMinor;
}

uint32_t PcapFileHeader::ThisZone() const
{
    return this->Header()->thiszone;
}

void PcapFileHeader::ThisZone(uint32_t thisZone)
{
    this->Header()->thiszone = thisZone;
}

uint32_t PcapFileHeader::SigFigs() const
{
    return this->Header()->sigfigs;
}

void PcapFileHeader::SigFigs(uint32_t sigFigs)
{
    this->Header()->sigfigs = sigFigs;
}

uint32_t PcapFileHeader::SnapLen() const
{
    return this->Header()->snaplen;
}

void PcapFileHeader::SnapLen(uint32_t snapLen)
{
    this->Header()->snaplen = snapLen;
}

uint32_t PcapFileHeader::LinkType() const
{
    return this->Header()->linktype;
}

void PcapFileHeader::LinkType(uint32_t linktype)
{
    this->Header()->linktype = linktype;
}

struct pcap_file_header *PcapFileHeader::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct pcap_file_header *header = reinterpret_cast<struct pcap_file_header *>(data_ptr);
    return header;
}
} // namespace PacketBuilder
