#include <EntityService.hpp>
#include <net/ethernet.h>

namespace PacketEntity
{
StackableEntityPtr EntityService::ParsePcapHelper::Parse(const uint8_t *packet, std::size_t length)
{
    auto ethernetEntityPtr = ParseEthernet(packet, length);
    return ethernetEntityPtr;
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseEthernet(const uint8_t *packet, std::size_t length)
{
    // TODO
    throw std::runtime_error("Not implemented");
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseIpv4(const uint8_t *packet, std::size_t length)
{
    // TODO
    throw std::runtime_error("Not implemented");
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseUdp(const uint8_t *packet, std::size_t length)
{
    // TODO
    throw std::runtime_error("Not implemented");
}
} // namespace PacketEntity
