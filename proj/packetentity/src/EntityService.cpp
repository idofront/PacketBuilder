#include <EntityService.hpp>
#include <fstream>
#include <pcap.h>

namespace PacketEntity
{
StackableEntityPtr TailEntity(StackableEntityPtr entity)
{
    if (entity->Stack.Value())
    {
        return TailEntity(entity->Stack.Value());
    }
    return entity;
}

StackableEntityPtr EntityService::ParseEntity(nlohmann::json json)
{
    auto tmpJson = json;
    auto entityPtr = BinaryEntityPtr(std::make_shared<BinaryEntity>(0));
    do
    {
        auto tailEntityPtr = TailEntity(entityPtr);

        auto type = tmpJson["EntityType"].get<std::string>();
        auto typeMap = std::map<std::string, std::function<StackableEntityPtr(nlohmann::json)>>{
            {Utility::Demangle(typeid(AbsoluteEntity).name()), AbsoluteEntity::FromJson},
            {Utility::Demangle(typeid(BinaryEntity).name()), BinaryEntity::FromJson},
            {Utility::Demangle(typeid(EthernetEntity).name()), EthernetEntity::FromJson},
            {Utility::Demangle(typeid(Ipv4Entity).name()), Ipv4Entity::FromJson},
            {Utility::Demangle(typeid(UdpEntity).name()), UdpEntity::FromJson},
        };
        auto factory = typeMap.find(type);
        if (factory == typeMap.end())
        {
            auto fmt = boost::format("Unknown entity type: %1%");
            auto msg = boost::str(fmt % type);
            throw std::runtime_error(msg);
        }

        auto stackedEntityPtr = factory->second(tmpJson);

        auto entityJson = stackedEntityPtr->ToJson();
        auto entityFmt = boost::format("Entity:\n%1%");
        auto entityMsg = boost::str(entityFmt % entityJson.dump(4));
        SPDLOG_TRACE(entityMsg);

        tailEntityPtr->Stack.Value(stackedEntityPtr);

        auto hasStack = tmpJson.find("Stack") != tmpJson.end();
        if (!hasStack)
        {
            break;
        }

        tmpJson = tmpJson["Stack"];
        auto nextEntityType = tmpJson["EntityType"].get<std::string>();
        auto fmt = boost::format("Next entity type: %1%") % nextEntityType;
        auto msg = boost::str(fmt);
        SPDLOG_DEBUG(msg);
    } while (true);

    return entityPtr->Stack.Value();
}

std::vector<StackableEntityPtr> EntityService::ParseEntities(const std::filesystem::path &filepath)
{
    auto inputStream = std::ifstream(filepath);
    auto json = nlohmann::json::parse(inputStream);

    auto entityPtrArray = std::vector<StackableEntityPtr>();
    auto array = json;

    if (!json.is_array())
    {
        // Array でなかったこと，Array とみなして処理を続行することを警告する
        auto fmt = boost::format("Expected array, but got %1%") % json.dump(4);
        auto msg = boost::str(fmt);
        SPDLOG_WARN(msg);

        array = nlohmann::json::array();
        array.push_back(json);
    }

    auto length = array.size();
    auto fmt = boost::format("Parsing %1% entities") % length;
    auto msg = boost::str(fmt);
    SPDLOG_DEBUG(msg);

    std::transform(array.begin(), array.end(), std::back_inserter(entityPtrArray),
                   [](nlohmann::json entity) { return EntityService::ParseEntity(entity); });

    return entityPtrArray;
}

StackableEntityPtr EntityService::ParsePcap(const struct pcap_pkthdr *const header, const uint8_t *packet)
{
    auto packet_length = header->caplen;

    auto entityPtr = ParsePcapHelper::Parse(packet, packet_length);

    auto absoluteEntityPtr = std::make_shared<AbsoluteEntity>();
    auto timestampNs = header->ts.tv_sec * 100'0000'000 + header->ts.tv_usec * 1000;
    absoluteEntityPtr->TimestampNs = timestampNs;

    absoluteEntityPtr->Stack.Value(entityPtr);

    return absoluteEntityPtr;
}

std::vector<StackableEntityPtr> EntityService::ParsePcap(const std::filesystem::path &filepath)
{
    auto entityPtrArray = std::vector<StackableEntityPtr>();

    // open pcap file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_open_offline(filepath.c_str(), errbuf);
    if (pcap_handle == nullptr)
    {
        auto fmt = boost::format("Error opening pcap file: %1%");
        auto msg = fmt % errbuf;
        throw std::runtime_error(msg.str());
    }

    // get each packet from pcap file
    struct pcap_pkthdr *header;
    const u_char *packet_data;
    int ret;

    while ((ret = pcap_next_ex(pcap_handle, &header, &packet_data)) >= 0)
    {
        if (ret == 0)
        {
            SPDLOG_INFO("Timeout occurred");
            continue;
        }

        // convert packet to entity
        auto entityPtr = ParsePcap(header, packet_data);
        entityPtrArray.push_back(entityPtr);
    }

    if (ret == -1)
    {
        auto fmt = boost::format("Error reading the packet: %1%");
        auto msg = fmt % pcap_geterr(pcap_handle);
        throw std::runtime_error(msg.str());
    }

    SPDLOG_INFO("Parsed {} packets", entityPtrArray.size());

    // close pcap file
    pcap_close(pcap_handle);

    return entityPtrArray;
}
} // namespace PacketEntity
