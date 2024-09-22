#include <EntityService.hpp>
#include <net/ethernet.h>

namespace PacketEntity
{
StackableEntityPtr EntityService::ParsePcapHelper::Parse(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = StackableEntityPtr();
    try
    {
        auto ethernetEntityPtr = ParseEthernet(packet, length);
        entityPtr = ethernetEntityPtr;
    }
    catch (const std::exception &e)
    {
        {
            auto fmt = boost::format("Failed to parse: %1%");
            auto msg = boost::str(fmt % e.what());
            SPDLOG_WARN(msg);
        }
        {
            auto fmt = boost::format("Falling back to binary entity.");
            auto msg = boost::str(fmt);
            SPDLOG_WARN(msg);
        }
        entityPtr = ParseBinary(packet, length);
    }
    return entityPtr;
}

EntityService::ParsePcapHelper::NextEthernetFactoryMap EntityService::ParsePcapHelper::NextEthernetFactory = {
    {ETHERTYPE_IP, EntityService::ParsePcapHelper::ParseIpv4},
};

StackableEntityPtr EntityService::ParsePcapHelper::ParseEthernet(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = std::make_shared<EthernetEntity>();

    auto etherHeader = reinterpret_cast<const ether_header *>(packet);
    auto etherType = ntohs(etherHeader->ether_type);
    auto etherSourceMac = etherHeader->ether_shost;
    auto etherDestinationMac = etherHeader->ether_dhost;
    entityPtr->Type = (etherType);
    Utility::EthernetAddressToString(entityPtr->SourceMac, etherSourceMac);
    Utility::EthernetAddressToString(entityPtr->DestinationMac, etherDestinationMac);

    auto nextFactoryItr = NextEthernetFactory.find(etherType);
    auto nextFactory = std::function<StackableEntityPtr(const uint8_t *, std::size_t)>(ParseBinary);
    try
    {
        if (nextFactoryItr == NextEthernetFactory.end())
        {
            auto fmt = boost::format("Unknown ether type: %1%");
            auto msg = boost::str(fmt % etherType);
            throw std::runtime_error(msg);
        }
        nextFactory = nextFactoryItr->second;
    }
    catch (const std::exception &e)
    {
        auto fmt = boost::format("Failed to parse: %1%");
        auto msg = boost::str(fmt % e.what());
        SPDLOG_WARN(msg);
    }

    auto nextEntity = StackableEntityPtr(nullptr);
    auto offset = sizeof(ether_header);
    try
    {
        // TODO #42 Ethernet Header の長さは可変のため，要修正
        nextEntity = nextFactory(packet + offset, length - offset);
    }
    catch (const std::exception &e)
    {
        {
            auto fmt = boost::format("Failed to parse: %1%");
            auto msg = boost::str(fmt % e.what());
            SPDLOG_WARN(msg);
        }
        {
            auto fmt = boost::format("Falling back to binary entity.");
            auto msg = boost::str(fmt);
            SPDLOG_WARN(msg);
        }
        nextEntity = ParseBinary(packet + offset, length - offset);
    }

    entityPtr->Stack.Value(nextEntity);
    return entityPtr;
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
