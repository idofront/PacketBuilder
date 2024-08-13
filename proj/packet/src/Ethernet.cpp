#include <Ethernet.hpp>
#include <arpa/inet.h>

namespace PacketBuilder
{
Ethernet::Ethernet() : Stackable(HeaderSize)
{
}

uint8_t *Ethernet::DestinationMac() const
{
    return this->Header()->ether_dhost;
}

void Ethernet::DestinationMac(uint8_t *destinationMac)
{
    memcpy(this->Header()->ether_dhost, destinationMac, 6);
}

uint8_t *Ethernet::SourceMac() const
{
    return this->Header()->ether_shost;
}

void Ethernet::SourceMac(uint8_t *sourceMac)
{
    memcpy(this->Header()->ether_shost, sourceMac, 6);
}

uint16_t Ethernet::EthernetType() const
{
    return this->Header()->ether_type;
}

void Ethernet::EthernetType(uint16_t ethernetType)
{
    this->Header()->ether_type = htons(ethernetType);
}

struct ether_header *Ethernet::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct ether_header *header = reinterpret_cast<struct ether_header *>(data_ptr);
    return header;
}
} // namespace PacketBuilder
