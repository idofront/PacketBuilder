#include <Udp.hpp>

class sample
{
  public:
    sample()
    {
        PacketBuilder::Udp udp;
        udp.SourcePort(0);
        udp.DestinationPort(0);
        udp.UdpLength(udp.Length());
        udp.UdpChecksum(0);
    }
};
