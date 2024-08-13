#ifndef PACKET_BUILDER__ETHERNET_HEADER_HPP__
#define PACKET_BUILDER__ETHERNET_HEADER_HPP__

#include <Stackable.hpp>
#include <netinet/ether.h>

namespace PacketBuilder
{
class Ethernet;
using EthernetPtr = std::shared_ptr<Ethernet>;

class Ethernet : public Stackable
{
  public:
    Ethernet();
    uint8_t *DestinationMac() const;
    void DestinationMac(uint8_t *destinationMac);
    uint8_t *SourceMac() const;
    void SourceMac(uint8_t *sourceMac);
    uint16_t EthernetType() const;
    void EthernetType(uint16_t ethernetType);

  private:
    static const std::size_t HeaderSize = 14;
    struct ether_header *Header() const;
};
} // namespace PacketBuilder
#endif
