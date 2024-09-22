#include <EntityService.hpp>
#include <net/ethernet.h>

namespace PacketEntity
{
StackableEntityPtr EntityService::ParsePcapHelper::Parse(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = ParseBinary(packet, length);
    return entityPtr;
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

StackableEntityPtr EntityService::ParsePcapHelper::ParseBinary(const uint8_t *packet, std::size_t length)
{
    auto binaryEntityPtr = std::make_shared<BinaryEntity>(length);
    memcpy(binaryEntityPtr->Data->data(), packet, length);
    return binaryEntityPtr;
}
} // namespace PacketEntity
