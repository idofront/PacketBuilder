#ifndef ETHERNET_PLUGIN__PARSER__ETHERNET_BINARY_PARSER_HPP
#define ETHERNET_PLUGIN__PARSER__ETHERNET_BINARY_PARSER_HPP

#include <Packet/Binary.hpp>
#include <Packet/Ethernet.hpp>
#include <Parser/BinaryParser.hpp>
#include <exception/InvalidOperationException.hpp>
#include <netinet/ether.h>

namespace EthernetPlugin
{
namespace Parser
{
class EthernetBinaryParser : public PluginContract::Parser::BinaryParser
{
  public:
    EthernetBinaryParser()
    {
    }
    virtual ~EthernetBinaryParser() override
    {
    }

  protected:
    virtual PluginContract::Packet::StackablePtr ParseImple() override
    {
        auto tail = this->TailStackable();

        auto binary = std::dynamic_pointer_cast<PluginContract::Packet::Binary>(tail);
        if (binary == nullptr)
        {
            throw Utility::Exception::InvalidOperationException("The tail stackable is not Binary.");
        }

        if (binary->Length() < sizeof(ether_header))
        {
            throw Utility::Exception::InvalidOperationException("The length of the binary is too short.");
        }

        auto header = reinterpret_cast<ether_header *>(binary->DataArray().get());
        auto ethernet = std::make_shared<EthernetPlugin::Packet::Ethernet>(header);

        auto ethernetLength = ethernet->Length();
        auto newBinary = std::make_shared<PluginContract::Packet::Binary>(binary->DataArray().get() + ethernetLength,
                                                                          binary->Length() - ethernetLength);
        ethernet->Stack.Value(ethernet);

        return newBinary;
    }
};

} // namespace Parser
} // namespace EthernetPlugin

#endif
