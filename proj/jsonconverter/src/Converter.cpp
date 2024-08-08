#include <Converter.hpp>
#include <arpa/inet.h>

namespace JsonConverter
{

void to_json(nlohmann::json &j, const StackableEntity &entity)
{
    // TODO not implemented
    throw std::runtime_error("Not implemented");
}

void from_json(const nlohmann::json &j, StackableEntity &entity)
{
    // TODO not implemented
    throw std::runtime_error("Not implemented");
}

void to_entity(StackableEntity &entity, const PacketBuilder::Stackable &header)
{
    // TODO not implemented
    throw std::runtime_error("Not implemented");
}

void from_entity(const StackableEntity &entity, PacketBuilder::Stackable &header)
{
    // TODO not implemented
    throw std::runtime_error("Not implemented");
}

void to_json(nlohmann::json &j, const EthernetHeaderEntity &entity)
{
    auto stack = entity.Stack ? nlohmann::json(*entity.Stack) : nlohmann::json(nullptr);
    j = nlohmann::json{{"DestinationMac", entity.DestinationMac},
                       {"SourceMac", entity.SourceMac},
                       {"EthernetType", entity.EthernetType},
                       {"StackType", entity.StackType},
                       {"Stack", stack}};
}

void from_json(const nlohmann::json &j, EthernetHeaderEntity &entity)
{
    j.at("DestinationMac").get_to(entity.DestinationMac);
    j.at("SourceMac").get_to(entity.SourceMac);
    j.at("EthernetType").get_to(entity.EthernetType);
    j.at("StackType").get_to(entity.StackType);

    if (entity.StackType.empty())
    {
        entity.Stack = nullptr;
    }
    else
    {
        auto stackJson = j.at("Stack");
        auto stackableEntityPtr = CreateStackableEntity(entity.StackType);
        // *stackableEntityPtr =
        stackJson.get<EthernetHeaderEntity>();

        entity.Stack = stackableEntityPtr;
    }
}

void to_entity(EthernetHeaderEntity &entity, const PacketBuilder::EthernetHeader &header)
{
    auto destMac = std::string("");
    EthernetAddressToString(destMac, header.DestinationMac());
    auto srcMac = std::string("");
    EthernetAddressToString(srcMac, header.SourceMac());
    entity.DestinationMac = destMac;
    entity.SourceMac = srcMac;
    entity.EthernetType = ntohs(header.EthernetType());

    if (header.Stack())
    {
        entity.StackType = typeid(*(header.Stack())).name();

        auto stackable = *header.Stack();
        auto stackableEntityPtr = CreateStackableEntity(entity.StackType);
        to_entity(*stackableEntityPtr, stackable);
    }
    else
    {
        entity.StackType = "";
        entity.Stack = nullptr;
    }
}

void from_entity(const EthernetHeaderEntity &entity, PacketBuilder::EthernetHeader &header)
{
}

void EthernetAddressToString(std::string &addr, const uint8_t *const ether_addr)
{
    char buf[18];
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", ether_addr[0], ether_addr[1], ether_addr[2], ether_addr[3],
            ether_addr[4], ether_addr[5]);
    addr = std::string(buf);
}

void EthernetAddressFromString(const std::string &addr, uint8_t *const ether_addr)
{
    std::sscanf(addr.c_str(), "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", &ether_addr[0], &ether_addr[1],
                &ether_addr[2], &ether_addr[3], &ether_addr[4], &ether_addr[5]);
}

StackableEntityPtr CreateStackableEntity(const std::string &type)
{
    auto map = std::map<std::string, std::function<StackableEntityPtr(void)>>{
        {typeid(PacketBuilder::EthernetHeader).name(), []() { return std::make_shared<EthernetHeaderEntity>(); }},
        {typeid(PacketBuilder::Ipv4).name(), []() { return std::make_shared<Ipv4Entity>(); }},
        {typeid(PacketBuilder::Udp).name(), []() { return std::make_shared<UdpEntity>(); }},
        {typeid(PacketBuilder::Stackable).name(), []() { return std::make_shared<StackableEntity>(); }},

    };
    auto iterator = map.find(type);
    if (iterator == map.end())
    {
        throw std::runtime_error("Unknown type");
    }
    return iterator->second();
}

StackableEntityPtr CreateStackableEntity(const std::string &type, const nlohmann::json &j)
{
    auto map = std::map<std::string, std::function<StackableEntityPtr()>>{
        {typeid(PacketBuilder::EthernetHeader).name(),
         [&j]() {
             auto entity = j.get<EthernetHeaderEntity>();
             auto entityPtr = std::make_shared<EthernetHeaderEntity>(entity);
             return entityPtr;
         }},
        {typeid(PacketBuilder::Ipv4).name(),
         [&j]() {
             auto entity = j.get<Ipv4Entity>();
             auto entityPtr = std::make_shared<Ipv4Entity>(entity);
             return entityPtr;
         }},
        {typeid(PacketBuilder::Udp).name(),
         [&j]() {
             auto entity = j.get<UdpEntity>();
             auto entityPtr = std::make_shared<UdpEntity>(entity);
             return entityPtr;
         }},
        {typeid(PacketBuilder::Stackable).name(),
         [&j]() {
             auto entity = j.get<StackableEntity>();
             auto entityPtr = std::make_shared<StackableEntity>(entity);
             return entityPtr;
         }},
    };

    if (map.find(type) == map.end())
    {
        throw std::runtime_error("Unknown type");
    }

    return map[type]();
}
} // namespace JsonConverter
