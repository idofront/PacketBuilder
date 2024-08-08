#include <EthernetHeader.hpp>
#include <arpa/inet.h>

namespace PacketBuilder
{
EthernetHeader::EthernetHeader() : Stackable(HeaderSize)
{
}

uint8_t *EthernetHeader::DestinationMac() const
{
    return this->Header()->ether_dhost;
}

void EthernetHeader::DestinationMac(uint8_t *destinationMac)
{
    memcpy(this->Header()->ether_dhost, destinationMac, 6);
}

uint8_t *EthernetHeader::SourceMac() const
{
    return this->Header()->ether_shost;
}

void EthernetHeader::SourceMac(uint8_t *sourceMac)
{
    memcpy(this->Header()->ether_shost, sourceMac, 6);
}

uint16_t EthernetHeader::EthernetType() const
{
    return this->Header()->ether_type;
}

void EthernetHeader::EthernetType(uint16_t ethernetType)
{
    this->Header()->ether_type = htons(ethernetType);
}

struct ether_header *EthernetHeader::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct ether_header *header = reinterpret_cast<struct ether_header *>(data_ptr);
    return header;
}
} // namespace PacketBuilder
