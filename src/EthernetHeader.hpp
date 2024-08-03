#ifndef PACKET_BUILDER__ETHERNET_HEADER_HPP__
#define PACKET_BUILDER__ETHERNET_HEADER_HPP__

#include <Stackable.hpp>
#include <netinet/ether.h>

namespace PacketBuilder
{
class EthernetHeader;
using EthernetHeaderPtr = std::shared_ptr<EthernetHeader>;

class EthernetHeader : public Stackable
{
  public:
    EthernetHeader();
    uint8_t *DestinationMac();
    void DestinationMac(uint8_t *destinationMac);
    uint8_t *SourceMac();
    void SourceMac(uint8_t *sourceMac);
    uint16_t EthernetType();
    void EthernetType(uint16_t ethernetType);

  private:
    static const std::size_t HeaderSize = 14;
    struct ether_header *Header() const;
};
} // namespace PacketBuilder
#endif
