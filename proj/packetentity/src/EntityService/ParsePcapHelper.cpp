#include <EntityService.hpp>
#include <net/ethernet.h>

namespace PacketEntity
{
StackableEntityPtr EntityService::ParsePcapHelper::Parse(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = StackableEntityPtr();
    try
    {
        // Absolute は EntityService::ParsePcap で処理されているため，ParsePcapHelper では Ethernet から処理する．
        auto ethernetEntityPtr = ParseEthernet(packet, length);
        entityPtr = ethernetEntityPtr;
    }
    catch (const std::exception &e)
    {
        // Ethernet の解析に失敗した場合，Binary として処理する．
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

    // TODO #42 Ethernet Header の長さは可変のため，要修正
    auto offset = sizeof(ether_header);
    auto nextEntity = CreateStackableEntity(nextFactory, packet + offset, length - offset);
    entityPtr->Stack.Value(nextEntity);

    return entityPtr;
}

StackableEntityPtr EntityService::ParsePcapHelper::CreateStackableEntity(FactoryFunction &factory,
                                                                         const uint8_t *packet,
                                                                         std::size_t length) noexcept
{
    auto nextEntity = StackableEntityPtr(nullptr);
    try
    {
        nextEntity = factory(packet, length);
    }
    catch (const std::exception &e)
    {
        {
            auto fmt = boost::format("Failed to parse: %1%");
            auto msg = boost::str(fmt % e.what());
            SPDLOG_WARN(msg);
        }
        {
            auto fmt = boost::format("Falling back from Ethernet to binary entity.");
            auto msg = boost::str(fmt);
            SPDLOG_WARN(msg);
        }
        nextEntity = ParseBinary(packet, length);
    }

    return nextEntity;
}

EntityService::ParsePcapHelper::NextIpv4FactoryMap EntityService::ParsePcapHelper::NextIpFactory = {
    {IPPROTO_UDP, EntityService::ParsePcapHelper::ParseUdp},
    {IPPROTO_TCP, EntityService::ParsePcapHelper::ParseTcp},
};

StackableEntityPtr EntityService::ParsePcapHelper::ParseIpv4(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = std::make_shared<Ipv4Entity>();

    if (length < 1)
    {
        throw std::runtime_error("Length is too short to parse IHL.");
    }

    entityPtr->Version = (packet[0] >> 4);
    entityPtr->IHL = (packet[0] & 0x0F) * 4;

    if (length < entityPtr->IHL)
    {
        auto fmt = boost::format("Length is lesser than IHL: %1% < %2%");
        auto msg = boost::str(fmt % length % entityPtr->IHL);
        throw std::runtime_error(msg);
    }

    entityPtr->DSCP = (packet[1] >> 2);
    entityPtr->ECN = (packet[1] & 0x03);
    entityPtr->TotalLength = ntohs(*reinterpret_cast<const uint16_t *>(packet + 2));
    entityPtr->Identification = ntohs(*reinterpret_cast<const uint16_t *>(packet + 4));
    entityPtr->Flags = (packet[6] >> 5);
    entityPtr->FragmentOffset = ntohs(*reinterpret_cast<const uint16_t *>(packet + 6)) & 0x1FFF;
    entityPtr->TTL = packet[8];
    entityPtr->Protocol = packet[9];
    entityPtr->HeaderChecksum = ntohs(*reinterpret_cast<const uint16_t *>(packet + 10));
    entityPtr->SourceAddress = *reinterpret_cast<const uint32_t *>(packet + 12);
    entityPtr->DestinationAddress = *reinterpret_cast<const uint32_t *>(packet + 16);

    auto nextFactoryItr = NextIpFactory.find(entityPtr->Protocol);
    auto nextFactory = std::function<StackableEntityPtr(const uint8_t *, std::size_t)>(ParseBinary);
    try
    {
        if (nextFactoryItr == NextIpFactory.end())
        {
            auto fmt = boost::format("Unknown protocol: %1%");
            auto msg = boost::str(fmt % entityPtr->Protocol);
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

    auto offset = entityPtr->IHL;
    auto nextEntity = nextFactory(packet + offset, length - offset);

    entityPtr->Stack.Value(nextEntity);
    return entityPtr;
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseTcp(const uint8_t *packet, std::size_t length)
{
    // TODO
    throw std::runtime_error("Not implemented for TCP");
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseUdp(const uint8_t *packet, std::size_t length)
{
    // TODO
    throw std::runtime_error("Not implemented for UDP");
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseBinary(const uint8_t *packet, std::size_t length)
{
    auto binaryEntityPtr = std::make_shared<BinaryEntity>(length);
    memcpy(binaryEntityPtr->Data->data(), packet, length);
    return binaryEntityPtr;
}
} // namespace PacketEntity
