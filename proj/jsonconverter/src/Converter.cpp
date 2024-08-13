#include <Converter.hpp>
#include <arpa/inet.h>
#include <map>
#include <typeindex>

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

void to_entity(StackableEntity &entity, const Packet::Stackable &header)
{
    // TODO not implemented
    throw std::runtime_error("Not implemented");
}

void from_entity(const StackableEntity &entity, Packet::Stackable &header)
{
    // TODO not implemented
    throw std::runtime_error("Not implemented");
}

nlohmann::json GetStackEntityJson(const StackableEntity &entity)
{
    auto stackType = entity.StackType;
    if (stackType.empty())
    {
        return nlohmann::json(nullptr);
    }

    nlohmann::json stackEntityJson;

    if (stackType == typeid(Packet::Ethernet).name())
    {
        stackEntityJson = nlohmann::json(*(std::dynamic_pointer_cast<EthernetHeaderEntity>(entity.Stack)));
    }
    else if (stackType == typeid(Packet::Ipv4).name())
    {
        stackEntityJson = nlohmann::json(*(std::dynamic_pointer_cast<Ipv4Entity>(entity.Stack)));
    }
    else if (stackType == typeid(Packet::Udp).name())
    {
        stackEntityJson = nlohmann::json(*(std::dynamic_pointer_cast<UdpEntity>(entity.Stack)));
    }
    else if (stackType == typeid(Packet::Stackable).name())
    {
        stackEntityJson = nlohmann::json(*(std::dynamic_pointer_cast<StackableEntity>(entity.Stack)));
    }
    else
    {
        throw std::runtime_error("Unknown type");
    }

    return stackEntityJson;
}

void to_json(nlohmann::json &j, const EthernetHeaderEntity &entity)
{
    auto stack = GetStackEntityJson(entity);
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
        entity.Stack = stackableEntityPtr;
    }
}

void to_entity(EthernetHeaderEntity &entity, const Packet::Ethernet &header)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
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

        auto stackableEntityPtr = CreateStackableEntity(entity.StackType);
        SPDLOG_DEBUG("StackType: {}", typeid(*(header.Stack())).name());
        if (typeid(*(header.Stack())) == typeid(Packet::Ipv4))
        {
            to_entity(*std::dynamic_pointer_cast<Ipv4Entity>(stackableEntityPtr),
                      *std::dynamic_pointer_cast<Packet::Ipv4>(header.Stack()));
        }
        entity.Stack = stackableEntityPtr;
    }
    else
    {
        entity.StackType = "";
        entity.Stack = nullptr;
    }
}

void from_entity(const EthernetHeaderEntity &entity, Packet::Ethernet &header)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
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

void to_json(nlohmann::json &j, const Ipv4Entity &entity)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto stack = GetStackEntityJson(entity);
    j = nlohmann::json{{"StackType", entity.StackType}, {"Stack", stack}};
}

void from_json(const nlohmann::json &j, Ipv4Entity &entity)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

void from_entity(const Ipv4Entity &entity, Packet::Ipv4 &header)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

void to_entity(Ipv4Entity &entity, const Packet::Ipv4 &header)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

StackableEntityPtr CreateStackableEntity(const std::string &type)
{
    auto map = std::map<std::string, std::function<StackableEntityPtr(void)>>{
        {typeid(Packet::Ethernet).name(), []() { return std::make_shared<EthernetHeaderEntity>(); }},
        {typeid(Packet::Ipv4).name(), []() { return std::make_shared<Ipv4Entity>(); }},
        {typeid(Packet::Udp).name(), []() { return std::make_shared<UdpEntity>(); }},
        {typeid(Packet::Stackable).name(), []() { return std::make_shared<StackableEntity>(); }},

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
        {typeid(Packet::Ethernet).name(),
         [&j]() {
             auto entity = j.get<EthernetHeaderEntity>();
             auto entityPtr = std::make_shared<EthernetHeaderEntity>(entity);
             return entityPtr;
         }},
        {typeid(Packet::Ipv4).name(),
         [&j]() {
             auto entity = j.get<Ipv4Entity>();
             auto entityPtr = std::make_shared<Ipv4Entity>(entity);
             return entityPtr;
         }},
        {typeid(Packet::Udp).name(),
         [&j]() {
             auto entity = j.get<UdpEntity>();
             auto entityPtr = std::make_shared<UdpEntity>(entity);
             return entityPtr;
         }},
        {typeid(Packet::Stackable).name(),
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

void to_json(nlohmann::json &j, const UdpEntity &entity)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    j = nlohmann::json{{"StackType", entity.StackType}};
}

void from_json(const nlohmann::json &j, UdpEntity &entity)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

void to_entity(UdpEntity &entity, const Packet::Udp &header)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

void from_entity(const UdpEntity &entity, Packet::Udp &header)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}
} // namespace JsonConverter
