#ifndef PACKET_BUILDER__ETHERNET_HEADER_HPP__
#define PACKET_BUILDER__ETHERNET_HEADER_HPP__

#include <EthernetEntity.hpp>
#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <netinet/ether.h>

namespace Packet
{
class Ethernet;
using EthernetPtr = std::shared_ptr<Ethernet>;

class Ethernet : public Stackable
{
  public:
    Ethernet();
    virtual ~Ethernet();

    Utility::NotifyProperty<uint8_t *> DestinationMac;
    Utility::NotifyProperty<uint8_t *> SourceMac;
    Utility::NotifyProperty<uint16_t> EthernetType;

  private:
    static const std::size_t HeaderSize = 14;
    struct ether_header *Header() const;
    PacketEntity::EthernetEntityPtr Entity();
};
} // namespace Packet
#endif
