#ifndef PACKET_BUILDER__UDP_HPP__
#define PACKET_BUILDER__UDP_HPP__

#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <UdpEntity.hpp>
#include <netinet/udp.h>

namespace Packet
{
class Udp;
using UdpPtr = std::shared_ptr<Udp>;

class Udp : public Stackable
{
  public:
    Udp();
    Udp(PacketEntity::UdpEntityPtr entity);

    Utility::NotifyProperty<uint16_t> SourcePort;
    Utility::NotifyProperty<uint16_t> DestinationPort;
    Utility::NotifyProperty<uint16_t> UdpLength;
    Utility::NotifyProperty<uint16_t> UdpChecksum;

  protected:
    virtual void OnStacked(StackablePtr oldStackable, StackablePtr newStackable) override;

  private:
    static const std::size_t HeaderSize = sizeof(struct udphdr);
    struct udphdr *UdpHeader();
    void RegisterCallbacks();
    PacketEntity::UdpEntityPtr Entity();
};
} // namespace Packet

#endif
