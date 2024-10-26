#ifndef ETHERNET_PLUGIN__ETHERNET_HPP
#define ETHERNET_PLUGIN__ETHERNET_HPP

#include <ObservableProperty.hpp>
#include <Packet/Stackable.hpp>
#include <netinet/ether.h>

template <class T> using ObservableProperty = Utility::ObservableProperty<T>;

namespace EthernetPlugin
{
namespace Packet
{
class Ethernet : public PluginContract::Packet::Stackable
{
  public:
    Ethernet();
    virtual ~Ethernet() override;
    ObservableProperty<std::string> DestinationAddress;
    ObservableProperty<std::string> SourceAddress;
    ObservableProperty<uint16_t> Type;
    ObservableProperty<uint16_t> PayloadLength;

  private:
    void OnDestinationAddressChanged(std::string newValue, std::string oldValue);
    void OnSourceAddressChanged(std::string newValue, std::string oldValue);
    void OnTypeChanged(uint16_t newValue, uint16_t oldValue);
    void OnPayloadLengthChanged(uint16_t newValue, uint16_t oldValue);

    virtual void OnStacked(PluginContract::Packet::StackablePtr oldStackable,
                           PluginContract::Packet::StackablePtr newStackable) override;

    ether_header *Header();
};
} // namespace Packet
} // namespace EthernetPlugin

#endif
