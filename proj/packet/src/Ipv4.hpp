#ifndef PACKET_BUILDER__IPV4_HPP__
#define PACKET_BUILDER__IPV4_HPP__

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

    int Version();
    void Version(int version);
    int Ihl();
    void Ihl(int ihl);

    uint8_t Tos();
    void Tos(uint8_t tos);
    uint16_t TotalLength();
    void TotalLength(uint16_t totalLength);
    uint16_t Id();
    void Id(uint16_t id);
    uint16_t Flags();
    void Flags(uint16_t flags);
    uint8_t Ttl();
    void Ttl(uint8_t ttl);
    uint8_t Protocol();
    void Protocol(uint8_t protocol);
    uint16_t Checksum();
    void Checksum(uint16_t checksum);
    uint32_t SourceIp();
    void SourceIp(uint32_t sourceIp);
    void SourceIp(sockaddr_in sourceIp);
    uint32_t DestinationIp();
    void DestinationIp(uint32_t destinationIp);
    void DestinationIp(sockaddr_in destinationIp);

  protected:
    virtual void OnStacked() override;

  private:
    using iphdr_t = struct iphdr;
    static const std::size_t HeaderSize = sizeof(iphdr_t);
    iphdr_t *Ipv4Header() const;
    static uint16_t CalculateChecksum(const Ipv4 *ipv4);
};
} // namespace Packet

#endif
