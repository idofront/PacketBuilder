#include <EntityService.hpp>

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

StackableEntityPtr EntityService::FromJson(nlohmann::json json)
{
    auto tmpJson = json;
    auto entityPtr = BinaryEntityPtr(std::make_shared<BinaryEntity>(0));
    do
    {
        auto tailEntityPtr = TailEntity(entityPtr);

        auto type = tmpJson["EntityType"].get<std::string>();
        auto typeMap = std::map<std::string, std::function<StackableEntityPtr(nlohmann::json)>>{
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

        auto stackedEntityPtr = factory->second(json);

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
    } while (true);

    auto entityJson = entityPtr->ToJson();
    auto entityFmt = boost::format("Entity:\n%1%");
    auto entityMsg = boost::str(entityFmt % entityJson.dump(4));
    SPDLOG_DEBUG(entityMsg);

    return entityPtr->Stack.Value();
}
} // namespace PacketEntity
