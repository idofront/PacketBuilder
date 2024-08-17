#include <Udp.hpp>
#include <UdpEntity.hpp>
#include <arpa/inet.h>

namespace Packet
{

Udp::Udp()
    : Stackable(HeaderSize, std::make_shared<PacketEntity::UdpEntity>()), SourcePort(0), DestinationPort(0),
      UdpLength(0), UdpChecksum(0)
{
    RegisterCallbacks();

    this->SourcePort.Value(0);
    this->DestinationPort.Value(0);
    this->UdpLength.Value(this->Length());
    this->UdpChecksum.Value(0);
}

Udp::Udp(PacketEntity::UdpEntityPtr entity)
    : Stackable(HeaderSize, entity), SourcePort(0), DestinationPort(0), UdpLength(0), UdpChecksum(0)
{
    RegisterCallbacks();

    this->SourcePort.Value(entity->SourcePort);
    this->DestinationPort.Value(entity->DestinationPort);
    this->UdpLength.Value(entity->Length);
    this->UdpChecksum.Value(entity->Checksum);
}

Udp::~Udp()
{
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
    UdpLength.Value(totalLength);

    // TODO Calculate checksum
}

void Udp::RegisterCallbacks()
{
    this->SourcePort.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->UdpHeader();
        header->source = htons(newValue);

        Entity()->SourcePort = newValue;
    });

    this->DestinationPort.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->UdpHeader();
        header->dest = htons(newValue);

        Entity()->DestinationPort = newValue;
    });

    this->UdpLength.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->UdpHeader();
        header->len = htons(newValue);

        Entity()->Length = newValue;
    });

    this->UdpChecksum.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->UdpHeader();
        header->check = htons(newValue);

        Entity()->Checksum = newValue;
    });
}

PacketEntity::UdpEntityPtr Udp::Entity()
{
    return std::dynamic_pointer_cast<PacketEntity::UdpEntity>(this->StackableEntity());
}
} // namespace Packet
