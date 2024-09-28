#include <EntityService.hpp>
#include <EthernetDot1qEntity.hpp>
#include <PortPair.hpp>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

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

EthernetDot1qEntity::Dot1qHeader GetDot1qHeader(const uint8_t *packet, std::size_t length)
{
    auto dot1qHeader = EthernetDot1qEntity::Dot1qHeader();

    auto size = sizeof(EthernetDot1qEntity::Dot1qHeader::TagControl);
    if (length < size)
    {
        auto fmt = boost::format("Length is too short to parse Dot1q header: %1%");
        auto msg = boost::str(fmt % length);
        throw std::runtime_error(msg);
    }

    auto userPriority = packet[0] >> 5;
    auto canonicalFormatIndicator = (packet[0] >> 4) & 0x01;
    auto vlanIdentifier = ((packet[0] & 0x0F) << 8) | packet[1];

    dot1qHeader.Tpid = ntohs(*reinterpret_cast<const uint16_t *>(packet));
    dot1qHeader.TagControl.UserPriority = userPriority;
    dot1qHeader.TagControl.CanonicalFormat = canonicalFormatIndicator;
    dot1qHeader.TagControl.VlanIdentifier = vlanIdentifier;

    return dot1qHeader;
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseEthernet(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = std::make_shared<EthernetEntity>();

    auto etherHeader = reinterpret_cast<const ether_header *>(packet);
    auto etherType = ntohs(etherHeader->ether_type);
    auto macHeaderLength = sizeof(ether_header);

    // TODO #50 VLAN タグがある場合のテスト

    auto etherSourceMac = etherHeader->ether_shost;
    auto etherDestinationMac = etherHeader->ether_dhost;
    auto offset = sizeof(ether_header);

    if (etherType == ETHERTYPE_VLAN)
    {
        auto dot1qHeader = GetDot1qHeader(packet + macHeaderLength * 2, length - macHeaderLength * 2);
        auto entityDot1qEntity = std::make_shared<EthernetDot1qEntity>();
        entityDot1qEntity->VlanHeader = dot1qHeader;
        entityPtr = entityDot1qEntity;

        auto tagProtocolIdentifier = dot1qHeader.Tpid;
        auto userPriority = dot1qHeader.TagControl.UserPriority;
        auto canonicalFormatIndicator = dot1qHeader.TagControl.CanonicalFormat;
        auto vlanIdentifier = dot1qHeader.TagControl.VlanIdentifier;

        dot1qHeader.Tpid = ntohs(tagProtocolIdentifier);
        dot1qHeader.TagControl.UserPriority = userPriority;
        dot1qHeader.TagControl.CanonicalFormat = canonicalFormatIndicator;
        dot1qHeader.TagControl.VlanIdentifier = vlanIdentifier;

        // VLAN タグが存在する場合，EtherType は VLAN タグの後にある
        auto etherTypeWithVlanOffset = sizeof(macHeaderLength) * 2 + sizeof(ether_header::ether_type);
        etherType = ntohs(*reinterpret_cast<const uint16_t *>(packet + etherTypeWithVlanOffset));

        // VLAN タグの長さを考慮してオフセットを更新する
        offset += sizeof(EthernetDot1qEntity::Dot1qHeader::TagControl_u::Value);
    }
    else
    {
        entityPtr = std::make_shared<EthernetEntity>();
    }

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
    entityPtr->IHL = (packet[0] & 0x0F);

    if (length < entityPtr->IHL * 4)
    {
        auto fmt = boost::format("Length is lesser than IHL: %1% < %2%");
        auto msg = boost::str(fmt % length % (entityPtr->IHL * 4));
        throw std::runtime_error(msg);
    }

    auto ip_header = reinterpret_cast<const iphdr *>(packet);

    entityPtr->DSCP = (ip_header->tos >> 2);
    entityPtr->ECN = (ip_header->tos & 0x03);
    entityPtr->TotalLength = ntohs(ip_header->tot_len);
    entityPtr->Identification = ntohs(ip_header->id);
    entityPtr->Flags = ntohs(ip_header->frag_off & 0xE000);
    entityPtr->FragmentOffset = ntohs(ip_header->frag_off & 0x1FFF);
    entityPtr->TTL = ip_header->ttl;
    entityPtr->Protocol = ip_header->protocol;
    entityPtr->HeaderChecksum = ntohs(ip_header->check);
    entityPtr->SourceAddress = inet_ntoa(*reinterpret_cast<const in_addr *>(packet + 12));
    entityPtr->DestinationAddress = inet_ntoa(*reinterpret_cast<const in_addr *>(packet + 16));

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

    auto offset = entityPtr->IHL * 4;
    auto nextEntity = StackableEntityPtr(nullptr);
    try
    {
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

EntityService::ParsePcapHelper::NextTransportFactoryMap EntityService::ParsePcapHelper::NextTransportFactory = {};

StackableEntityPtr EntityService::ParsePcapHelper::ParseTcp(const uint8_t *packet, std::size_t length)
{
    // TODO
    throw std::runtime_error("Not implemented for TCP");
}

StackableEntityPtr EntityService::ParsePcapHelper::ParseUdp(const uint8_t *packet, std::size_t length)
{
    auto entityPtr = std::make_shared<UdpEntity>();

    if (length < 8)
    {
        auto fmt = boost::format("Length is too short to parse UDP header: %1%");
        auto msg = boost::str(fmt % length);
        throw std::runtime_error(msg);
    }

    auto udpHeader = reinterpret_cast<const struct udphdr *>(packet);

    entityPtr->SourcePort = ntohs(udpHeader->source);
    entityPtr->DestinationPort = ntohs(udpHeader->dest);
    entityPtr->Length = ntohs(udpHeader->len);
    entityPtr->Checksum = ntohs(udpHeader->check);

    // TODO Application Layer の解析
    auto portPair = Utility::PortPair(entityPtr->SourcePort, entityPtr->DestinationPort);
    auto nextFactoryItr = NextTransportFactory.find(portPair);
    auto nextFactory = std::function<StackableEntityPtr(const uint8_t *, std::size_t)>(ParseBinary);

    if (nextFactoryItr != NextTransportFactory.end())
    {
        nextFactory = nextFactoryItr->second;
    }
    else
    {
        nextFactory = ParseBinary;
    }

    auto offset = sizeof(struct udphdr);
    auto nextEntity = StackableEntityPtr(nullptr);
    try
    {
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

StackableEntityPtr EntityService::ParsePcapHelper::ParseBinary(const uint8_t *packet, std::size_t length)
{
    auto binaryEntityPtr = std::make_shared<BinaryEntity>(length);
    memcpy(binaryEntityPtr->Data->data(), packet, length);
    return binaryEntityPtr;
}
} // namespace PacketEntity
