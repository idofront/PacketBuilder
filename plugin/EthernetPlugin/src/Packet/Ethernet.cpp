#include <Packet/Ethernet.hpp>
#include <Utility.hpp>
#include <arpa/inet.h>
#include <boost/format.hpp>

namespace EthernetPlugin
{
namespace Packet
{
Ethernet::Ethernet()
    : PluginContract::Packet::Stackable(sizeof(ether_header)), DestinationAddress(""), SourceAddress(""), Type(0),
      PayloadLength(0)
{
    this->DestinationAddress.RegisterCallback(
        [this](std::string newValue, std::string oldValue) { this->OnDestinationAddressChanged(newValue, oldValue); });

    this->SourceAddress.RegisterCallback(
        [this](std::string newValue, std::string oldValue) { this->OnSourceAddressChanged(newValue, oldValue); });

    this->Type.RegisterCallback(
        [this](uint16_t newValue, uint16_t oldValue) { this->OnTypeChanged(newValue, oldValue); });

    this->PayloadLength.RegisterCallback(
        [this](uint16_t newValue, uint16_t oldValue) { this->OnPayloadLengthChanged(newValue, oldValue); });
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
}

void Ethernet::OnPayloadLengthChanged(uint16_t newValue, uint16_t oldValue)
{
    auto header = this->Header();
    header->ether_type = htons(newValue);
}

void Ethernet::OnStacked(PluginContract::Packet::StackablePtr oldStackable,
                         PluginContract::Packet::StackablePtr newStackable)
{
    auto format = boost::format("Re-calculating payload length: %1% -> %2%");
    auto msg = format % this->PayloadLength.Value() % this->GetTotalLength(newStackable);
    SPDLOG_INFO(msg.str());

    PluginContract::Packet::Stackable::OnStacked(oldStackable, newStackable);
    this->PayloadLength.Value(GetTotalLength(newStackable));
}

ether_header *Ethernet::Header()
{
    return reinterpret_cast<struct ether_header *>(this->DataArray().get());
}
} // namespace Packet
} // namespace EthernetPlugin
