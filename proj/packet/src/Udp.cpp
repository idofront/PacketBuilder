#include <Udp.hpp>
#include <UdpEntity.hpp>
#include <arpa/inet.h>

namespace Packet
{

Udp::Udp() : Stackable(HeaderSize, std::make_shared<PacketEntity::UdpEntity>())
{
    this->SourcePort(0);
    this->DestinationPort(0);
    this->UdpLength(this->Length());
    this->UdpChecksum(0);
}

uint16_t Udp::SourcePort()
{
    return this->UdpHeader()->uh_sport;
}

void Udp::SourcePort(uint16_t sourcePort)
{
    this->UdpHeader()->uh_sport = htons(sourcePort);
}

uint16_t Udp::DestinationPort()
{
    return this->UdpHeader()->uh_dport;
}

void Udp::DestinationPort(uint16_t destinationPort)
{
    this->UdpHeader()->uh_dport = htons(destinationPort);
}

uint16_t Udp::UdpLength()
{
    return this->UdpHeader()->uh_ulen;
}

void Udp::UdpLength(uint16_t udpLength)
{
    this->UdpHeader()->uh_ulen = htons(udpLength);
}

uint16_t Udp::UdpChecksum()
{
    return this->UdpHeader()->uh_sum;
}

void Udp::UdpChecksum(uint16_t udpChecksum)
{
    this->UdpHeader()->uh_sum = htons(udpChecksum);
}

struct udphdr *Udp::UdpHeader()
{
    auto data_ptr = this->DataArray().get();
    struct udphdr *udpHeader = reinterpret_cast<struct udphdr *>(data_ptr);
    return udpHeader;
}

void Udp::OnStacked(StackablePtr oldStackable, StackablePtr newStackable)
{
    Stackable::OnStacked(oldStackable, newStackable);

    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto totalLength = Stackable::GetTotalLength(this->Stack.Value()) + this->Length();
    UdpLength(totalLength);

    // TODO Calculate checksum
}
} // namespace Packet
