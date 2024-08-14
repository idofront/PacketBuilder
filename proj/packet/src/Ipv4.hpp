#ifndef PACKET_BUILDER__IPV4_HPP__
#define PACKET_BUILDER__IPV4_HPP__

#include <Ipv4Entity.hpp>
#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <netinet/ip.h>

namespace Packet
{
class Ipv4;
using Ipv4Ptr = std::shared_ptr<Ipv4>;

class Ipv4 : public Stackable
{
  public:
    Ipv4();

    Utility::NotifyProperty<int> Version;
    Utility::NotifyProperty<int> Ihl;

    Utility::NotifyProperty<uint8_t> Tos;
    Utility::NotifyProperty<uint16_t> TotalLength;
    Utility::NotifyProperty<uint16_t> Id;
    Utility::NotifyProperty<uint16_t> Flags;
    Utility::NotifyProperty<uint8_t> Ttl;
    Utility::NotifyProperty<uint8_t> Protocol;
    Utility::NotifyProperty<uint16_t> Checksum;
    Utility::NotifyProperty<sockaddr_in> SourceAddress;
    Utility::NotifyProperty<sockaddr_in> DestinationAddress;

  protected:
    virtual void OnStacked() override;

  private:
    using iphdr_t = struct iphdr;
    static const std::size_t HeaderSize = sizeof(iphdr_t);
    iphdr_t *Ipv4Header() const;
    static uint16_t CalculateChecksum(const Ipv4 *ipv4);
    PacketEntity::Ipv4EntityPtr Entity();
};
} // namespace Packet

#endif
