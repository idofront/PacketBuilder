#include <Ipv4.hpp>
#include <arpa/inet.h>

namespace Packet
{
Ipv4::Ipv4()
    : Stackable(HeaderSize, std::make_shared<PacketEntity::Ipv4Entity>()), Version(0), Ihl(0), Tos(0), TotalLength(0),
      Id(0), Flags(0), Ttl(0), Protocol(0), Checksum(0), SourceAddress({0}), DestinationAddress({0})
{
    RegisterCallbacks();

    struct sockaddr_in sourceAddress;
    struct sockaddr_in destinationAddress;
    inet_pton(AF_INET, "0.0.0.0", &(sourceAddress.sin_addr));
    inet_pton(AF_INET, "0.0.0.0", &(destinationAddress.sin_addr));

    this->Version.Value(4);
    this->Ihl.Value(5);
    this->Tos.Value(0);
    this->TotalLength.Value(this->Length());
    this->Id.Value(0);
    this->Flags.Value(0);
    this->Ttl.Value(64);
    this->Protocol.Value(IPPROTO_UDP);
    this->Checksum.Value(0);
    this->SourceAddress.Value(sourceAddress);
    this->DestinationAddress.Value(destinationAddress);
}

Ipv4::Ipv4(PacketEntity::Ipv4EntityPtr entity)
    : Stackable(HeaderSize, entity), Version(entity->Version), Ihl(entity->IHL), Tos(entity->DSCP << 2 | entity->ECN),
      TotalLength(entity->TotalLength), Id(entity->Identification), Flags(entity->Flags), Ttl(entity->TTL),
      Protocol(entity->Protocol), Checksum(entity->HeaderChecksum), SourceAddress({0}), DestinationAddress({0})
{
    RegisterCallbacks();

    auto header = this->Ipv4Header();

    this->Version.Value(entity->Version);
    this->Ihl.Value(entity->IHL);
    this->Tos.Value(entity->DSCP << 2 | entity->ECN);
    this->TotalLength.Value(entity->TotalLength);
    this->Id.Value(entity->Identification);
    this->Flags.Value(entity->Flags);
    this->Ttl.Value(entity->TTL);
    this->Protocol.Value(entity->Protocol);
    this->Checksum.Value(entity->HeaderChecksum);

    auto srcSockaddrIn = sockaddr_in();
    srcSockaddrIn.sin_addr.s_addr = htonl(entity->SourceAddress);
    this->SourceAddress.Value(srcSockaddrIn);

    auto dstSockaddrIn = sockaddr_in();
    dstSockaddrIn.sin_addr.s_addr = htonl(entity->DestinationAddress);
    this->DestinationAddress.Value(dstSockaddrIn);
}

Ipv4::~Ipv4()
{
}

struct iphdr *Ipv4::Ipv4Header() const
{
    auto data_ptr = this->DataArray().get();
    iphdr_t *ipv4Header = reinterpret_cast<iphdr_t *>(data_ptr);
    return ipv4Header;
}

void Ipv4::OnStacked(StackablePtr oldStackable, StackablePtr newStackable)
{
    Stackable::OnStacked(oldStackable, newStackable);

    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto totalLength = Stackable::GetTotalLength(this->Stack.Value()) + this->Length();
    TotalLength.Value(totalLength);
    auto checksum = CalculateChecksum(this);
    SPDLOG_DEBUG("checksum: {:04X}", checksum);
    Checksum.Value(checksum);
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

PacketEntity::Ipv4EntityPtr Ipv4::Entity()
{
    auto entity = this->StackableEntity();
    return std::dynamic_pointer_cast<PacketEntity::Ipv4Entity>(entity);
}

void Ipv4::RegisterCallbacks()
{
    this->Version.RegisterCallback([this](int oldValue, int newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->version = newValue;

        Entity()->Version = newValue;
    });

    this->Ihl.RegisterCallback([this](int oldValue, int newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->ihl = newValue;

        Entity()->IHL = newValue;
    });

    this->Tos.RegisterCallback([this](uint8_t oldValue, uint8_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->tos = newValue;

        Entity()->DSCP = newValue >> 2;
        Entity()->ECN = newValue & 0x03;
    });

    this->TotalLength.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->tot_len = htons(newValue);

        Entity()->TotalLength = newValue;
    });

    this->Id.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->id = htons(newValue);

        Entity()->Identification = newValue;
    });

    this->Flags.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->frag_off = htons(newValue);

        Entity()->Flags = newValue;
    });

    this->Ttl.RegisterCallback([this](uint8_t oldValue, uint8_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->ttl = newValue;

        Entity()->TTL = newValue;
    });

    this->Protocol.RegisterCallback([this](uint8_t oldValue, uint8_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->protocol = newValue;

        Entity()->Protocol = newValue;
    });

    this->Checksum.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->check = htons(newValue);

        Entity()->HeaderChecksum = newValue;
    });

    this->SourceAddress.RegisterCallback([this](sockaddr_in oldValue, sockaddr_in newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->saddr = newValue.sin_addr.s_addr;

        Entity()->SourceAddress = ntohl(newValue.sin_addr.s_addr);
    });

    this->DestinationAddress.RegisterCallback([this](sockaddr_in oldValue, sockaddr_in newValue) {
        auto data = this->DataArray().get();
        auto header = this->Ipv4Header();
        header->daddr = newValue.sin_addr.s_addr;

        Entity()->DestinationAddress = ntohl(newValue.sin_addr.s_addr);
    });
}
} // namespace Packet
