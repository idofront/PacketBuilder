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

std::vector<StackableEntityPtr> EntityService::ParseEntities(nlohmann::json json)
{
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
} // namespace PacketEntity
