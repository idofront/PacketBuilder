#include <Ethernet.hpp>
#include <arpa/inet.h>

namespace Packet
{
Ethernet::Ethernet()
    : Stackable(HeaderSize, std::make_shared<PacketEntity::EthernetEntity>()), DestinationMac(new uint8_t[6]),
      SourceMac(new uint8_t[6]), EthernetType(0)
{
    RegisterCallbacks();
}

Ethernet::Ethernet(PacketEntity::EthernetEntityPtr entity)
    : Stackable(HeaderSize, entity), DestinationMac(new uint8_t[6]), SourceMac(new uint8_t[6]), EthernetType(0)
{
    RegisterCallbacks();

    auto header = this->Header();
    auto dhost = new uint8_t[6]{0, 0, 0, 0, 0, 0};
    auto shost = new uint8_t[6]{0, 0, 0, 0, 0, 0};
    Utility::EthernetAddressFromString(entity->DestinationMac, dhost);
    Utility::EthernetAddressFromString(entity->SourceMac, shost);

    DestinationMac.Value(dhost);
    SourceMac.Value(shost);
    EthernetType.Value(entity->Type);
}

Ethernet::~Ethernet()
{
}

struct ether_header *Ethernet::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct ether_header *header = reinterpret_cast<struct ether_header *>(data_ptr);
    return header;
}

PacketEntity::EthernetEntityPtr Ethernet::Entity()
{
    return std::dynamic_pointer_cast<PacketEntity::EthernetEntity>(this->StackableEntity());
}

void Ethernet::RegisterCallbacks()
{
    DestinationMac.RegisterCallback([this](uint8_t *oldValue, uint8_t *newValue) {
        auto data = this->DataArray().get();
        auto header = this->Header();
        memcpy(header->ether_dhost, newValue, 6);

        auto mac = std::string("");
        Utility::EthernetAddressToString(mac, newValue);
        Entity()->DestinationMac = mac;
    });

    SourceMac.RegisterCallback([this](uint8_t *oldValue, uint8_t *newValue) {
        auto data = this->DataArray().get();
        auto header = this->Header();
        memcpy(header->ether_shost, newValue, 6);

        auto mac = std::string("");
        Utility::EthernetAddressToString(mac, newValue);
        Entity()->SourceMac = mac;
    });

    EthernetType.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto data = this->DataArray().get();
        auto header = this->Header();
        header->ether_type = htons(newValue);

        Entity()->Type = newValue;
    });
}
} // namespace Packet
