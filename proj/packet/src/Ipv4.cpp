#include <Ipv4.hpp>
#include <Ipv4Entity.hpp>
#include <arpa/inet.h>

namespace Packet
{
Ipv4::Ipv4() : Stackable(HeaderSize, std::make_shared<PacketEntity::Ipv4Entity>())
{
    struct sockaddr_in sourceAddress;
    struct sockaddr_in destinationAddress;
    inet_pton(AF_INET, "0.0.0.0", &(sourceAddress.sin_addr));
    inet_pton(AF_INET, "0.0.0.0", &(destinationAddress.sin_addr));

    this->Version(4);
    this->Ihl(5);
    this->Tos(0);
    this->TotalLength(this->Length());
    this->Id(0);
    this->Flags(0);
    this->Ttl(64);
    this->Protocol(IPPROTO_UDP);
    this->Checksum(0);
    this->SourceIp(sourceAddress);
    this->DestinationIp(destinationAddress);
}

int Ipv4::Version()
{
    return this->Ipv4Header()->version;
}

void Ipv4::Version(int version)
{
    this->Ipv4Header()->version = version;
}

int Ipv4::Ihl()
{
    return this->Ipv4Header()->ihl;
}

void Ipv4::Ihl(int ihl)
{
    this->Ipv4Header()->ihl = ihl;
}

uint8_t Ipv4::Tos()
{
    return this->Ipv4Header()->tos;
}

void Ipv4::Tos(uint8_t tos)
{
    this->Ipv4Header()->tos = tos;
}

uint16_t Ipv4::TotalLength()
{
    return this->Ipv4Header()->tot_len;
}

void Ipv4::TotalLength(uint16_t totalLength)
{
    this->Ipv4Header()->tot_len = htons(totalLength);
}

uint16_t Ipv4::Id()
{
    return this->Ipv4Header()->id;
}

void Ipv4::Id(uint16_t id)
{
    this->Ipv4Header()->id = htons(id);
}

uint16_t Ipv4::Flags()
{
    return this->Ipv4Header()->frag_off;
}

void Ipv4::Flags(uint16_t flags)
{
    this->Ipv4Header()->frag_off = htons(flags);
}

uint8_t Ipv4::Ttl()
{
    return this->Ipv4Header()->ttl;
}

void Ipv4::Ttl(uint8_t ttl)
{
    this->Ipv4Header()->ttl = ttl;
}

uint8_t Ipv4::Protocol()
{
    return this->Ipv4Header()->protocol;
}

void Ipv4::Protocol(uint8_t protocol)
{
    this->Ipv4Header()->protocol = protocol;
}

uint16_t Ipv4::Checksum()
{
    return this->Ipv4Header()->check;
}

void Ipv4::Checksum(uint16_t checksum)
{
    this->Ipv4Header()->check = htons(checksum);
}

uint32_t Ipv4::SourceIp()
{
    return this->Ipv4Header()->saddr;
}

void Ipv4::SourceIp(uint32_t sourceIp)
{
    this->Ipv4Header()->saddr = htonl(sourceIp);
}

void Ipv4::SourceIp(sockaddr_in sourceIp)
{
    this->Ipv4Header()->saddr = sourceIp.sin_addr.s_addr;
}

uint32_t Ipv4::DestinationIp()
{
    return this->Ipv4Header()->daddr;
}

void Ipv4::DestinationIp(uint32_t destinationIp)
{
    this->Ipv4Header()->daddr = htonl(destinationIp);
}

void Ipv4::DestinationIp(sockaddr_in destinationIp)
{
    this->Ipv4Header()->daddr = destinationIp.sin_addr.s_addr;
}

struct iphdr *Ipv4::Ipv4Header() const
{
    auto data_ptr = this->DataArray().get();
    iphdr_t *ipv4Header = reinterpret_cast<iphdr_t *>(data_ptr);
    return ipv4Header;
}

void Ipv4::OnStacked()
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto totalLength = Stackable::GetTotalLength(this->Stack()) + this->Length();
    TotalLength(totalLength);
    auto checksum = CalculateChecksum(this);
    SPDLOG_DEBUG("checksum: {:04X}", checksum);
    Checksum(checksum);
}

uint16_t Ipv4::CalculateChecksum(const Ipv4 *ipv4)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    uint32_t sum = 0;
    auto header = ipv4->Ipv4Header();
    auto ihl = header->ihl;
    auto data = ipv4->DataArray().get();
    auto dataLength = ipv4->Length();

    iphdr_t temporaryHeader;
    memcpy(&temporaryHeader, header, sizeof(iphdr_t));
    if (temporaryHeader.check != 0)
    {
        SPDLOG_WARN("Checksum is not 0, setting it to 0");
        temporaryHeader.check = 0;
    }

    auto *dataPtr = reinterpret_cast<uint8_t *>(&temporaryHeader);
    for (int i = 0; i < ihl * 2; i++)
    {
        sum += dataPtr[i * 2] << 8 | dataPtr[i * 2 + 1];
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}
} // namespace Packet
