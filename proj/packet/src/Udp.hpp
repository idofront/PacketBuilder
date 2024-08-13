#ifndef PACKET_BUILDER__UDP_HPP__
#define PACKET_BUILDER__UDP_HPP__

#include <Stackable.hpp>
#include <Utility/Utility.hpp>
#include <netinet/udp.h>

namespace Packet
{
class Udp;
using UdpPtr = std::shared_ptr<Udp>;

class Udp : public Stackable
{
  public:
    Udp();
    uint16_t SourcePort();
    void SourcePort(uint16_t sourcePort);
    uint16_t DestinationPort();
    void DestinationPort(uint16_t destinationPort);
    uint16_t UdpLength();
    void UdpLength(uint16_t udpLength);
    uint16_t UdpChecksum();
    void UdpChecksum(uint16_t udpChecksum);

  protected:
    virtual void OnStacked() override;

  private:
    static const std::size_t HeaderSize = sizeof(struct udphdr);
    struct udphdr *UdpHeader();
};
} // namespace Packet

#endif
