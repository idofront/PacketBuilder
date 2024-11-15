#include <Packet/Ethernet.hpp>
#include <Utility.hpp>
#include <arpa/inet.h>
#include <boost/format.hpp>

namespace EthernetPlugin
{
namespace Packet
{
Ethernet::Ethernet()
    : PluginContract::Packet::Stackable(sizeof(ether_header)), DestinationAddress(""), SourceAddress(""), Type(0)
{
    this->DestinationAddress.RegisterCallback(
        [this](std::string newValue, std::string oldValue) { this->OnDestinationAddressChanged(newValue, oldValue); });

    this->SourceAddress.RegisterCallback(
        [this](std::string newValue, std::string oldValue) { this->OnSourceAddressChanged(newValue, oldValue); });

    this->Type.RegisterCallback(
        [this](uint16_t newValue, uint16_t oldValue) { this->OnTypeChanged(newValue, oldValue); });
}

Ethernet::Ethernet(const ether_header *const header) : Ethernet()
{
    auto destinationAddress = std::string("");
    Utility::EthernetAddressToString(destinationAddress, header->ether_dhost);

    auto sourceAddress = std::string("");
    Utility::EthernetAddressToString(sourceAddress, header->ether_shost);

    auto type = ntohs(header->ether_type);

    this->DestinationAddress.Value(destinationAddress);
    this->SourceAddress.Value(sourceAddress);
    this->Type.Value(type);
}

Ethernet::~Ethernet()
{
}

void Ethernet::OnDestinationAddressChanged(std::string newValue, std::string oldValue)
{
    auto header = this->Header();
    Utility::EthernetAddressFromString(newValue, header->ether_dhost);
}

void Ethernet::OnSourceAddressChanged(std::string newValue, std::string oldValue)
{
    auto header = this->Header();
    Utility::EthernetAddressFromString(newValue, header->ether_shost);
}

void Ethernet::OnTypeChanged(uint16_t newValue, uint16_t oldValue)
{
    auto header = this->Header();
    header->ether_type = htons(newValue);

    switch (newValue)
    {
    case ETHERTYPE_IP:
    case ETHERTYPE_ARP:
    case ETHERTYPE_REVARP:
    case ETHERTYPE_IPV6:
        break;
    case ETHERTYPE_VLAN:
        SPDLOG_WARN("Ethernet Type: VLAN is not supported. Probably, this packet could not be parsed.");
        break;
    default:
        SPDLOG_INFO("Unknown Ethernet Type: 0x{:04x} is set.", newValue);
        break;
    }
}

void Ethernet::OnStacked(PluginContract::Packet::StackablePtr oldStackable,
                         PluginContract::Packet::StackablePtr newStackable)
{
    PluginContract::Packet::Stackable::OnStacked(oldStackable, newStackable);
}

ether_header *Ethernet::Header()
{
    return reinterpret_cast<struct ether_header *>(this->DataArray().get());
}
} // namespace Packet
} // namespace EthernetPlugin
