#include <EntityService.hpp>

namespace PacketEntity
{
StackableEntityPtr EntityService::FromJson(nlohmann::json json)
{
    auto type = json["EntityType"].get<std::string>();
    auto typeMap = std::map<std::string, std::function<StackableEntityPtr(nlohmann::json)>>{
        // {Utility::Demangle(typeid(BinaryEntity).name()), BinaryEntity::FromJson},
        {Utility::Demangle(typeid(EthernetEntity).name()), EthernetEntity::FromJson},
        // {Utility::Demangle(typeid(Ipv4Entity).name()), Ipv4Entity::FromJson},
        // {Utility::Demangle(typeid(UdpEntity).name()), UdpEntity::FromJson},
    };
    auto factory = typeMap.find(type);
    if (factory == typeMap.end())
    {
        auto fmt = boost::format("Unknown entity type: %1%");
        auto msg = boost::str(fmt % type);
        throw std::runtime_error(msg);
    }

    auto entityPtr = factory->second(json);

    auto entityJson = entityPtr->ToJson();
    auto entityFmt = boost::format("Entity:\n%1%");
    auto entityMsg = boost::str(entityFmt % entityJson.dump(4));
    SPDLOG_DEBUG(entityMsg);

    return entityPtr;
}
} // namespace PacketEntity
