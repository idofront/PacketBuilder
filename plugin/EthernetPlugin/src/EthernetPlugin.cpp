#include <PluginInterface.hpp>
#include <Poco/ClassLibrary.h>
#include <arpa/inet.h>

#include <Packet/Ethernet.hpp>
#include <Parser/EthernetBinaryParser.hpp>
#include <spdlog/spdlog.h>

namespace EthernetPlugin
{
class EthernetPlugin : public PluginContract::PluginInterface
{
  public:
    EthernetPlugin()
    {
    }

    void ExecuteImpl() override
    {
        auto container = this->Container();

        auto binaryParserPtr = std::make_shared<Parser::EthernetBinaryParser>();

        auto parserCondition =
            PluginContract::PluginContainer::ParserCondition([](PluginContract::Packet::StackablePtr stackable) {
                auto binary = std::dynamic_pointer_cast<PluginContract::Packet::Binary>(stackable);
                if (binary == nullptr)
                {
                    auto fmt = boost::format("The tail stackable is not Binary.");
                    auto msg = fmt.str();
                    SPDLOG_INFO(msg);
                    return false;
                }

                if (binary->Length() < sizeof(ether_header))
                {
                    auto fmt = boost::format("The length of the binary is too short.");
                    auto msg = fmt.str();
                    SPDLOG_INFO(msg);
                    return false;
                }

                auto header = reinterpret_cast<ether_header *>(binary->DataArray().get());
                auto type = ntohs(header->ether_type);

                if (type != ETHERTYPE_IP && type != ETHERTYPE_IPV6)
                {
                    auto fmt = boost::format("The type of the ethernet is not IP or IPv6. Type: %1%");
                    auto msg = fmt % type;
                    SPDLOG_INFO(msg.str());
                    return false;
                }
                return true;
            });
        container->RegisterParser(binaryParserPtr, parserCondition);
    }
};
} // namespace EthernetPlugin

POCO_BEGIN_MANIFEST(PluginContract::PluginInterface)
POCO_EXPORT_CLASS(EthernetPlugin::EthernetPlugin)
POCO_END_MANIFEST
