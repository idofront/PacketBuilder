#include <AbsoluteFactory.hpp>
#include <BinaryFactory.hpp>
#include <EthernetFactory.hpp>
#include <Ipv4Factory.hpp>
#include <PacketService.hpp>
#include <UdpFactory.hpp>

namespace Packet
{
StackablePtr PacketService::StackableFromEntity(PacketEntity::StackableEntityPtr entity)
{
    auto stackableFactory = GetStackableFactory(entity);
    auto stackable = stackableFactory->Get(entity);

    auto stackedEntityPtr = entity->Stack.Value();
    if (stackedEntityPtr)
    {
        auto &stackedEntityRef = *stackedEntityPtr;
        auto stackedType = typeid(stackedEntityRef).name();
        auto demangledStackedType = Utility::Demangle(stackedType);
        auto fmt = boost::format("The entity has a stack: %1%");
        auto msg = boost::str(fmt % demangledStackedType);
        SPDLOG_DEBUG(msg);

        auto stackedStackable = StackableFromEntity(stackedEntityPtr);
        auto &stackedStackableRef = *stackedStackable;
        stackedType = typeid(stackedStackableRef).name();
        demangledStackedType = Utility::Demangle(stackedType);
        fmt = boost::format("Try to stack the entity: %1%");
        msg = boost::str(fmt % demangledStackedType);
        SPDLOG_DEBUG(msg);

        stackable->Stack.Value(stackedStackable);
    }

    return stackable;
}

StackableFactoryPtr PacketService::GetStackableFactory(PacketEntity::StackableEntityPtr entityPtr)
{
    try
    {
        auto factories = PacketService::StackableFactories;
        auto &entityRef = *entityPtr;
        auto type_name = typeid(entityRef).name();
        auto demangledType = Utility::Demangle(type_name);

        auto factory = factories.find(demangledType);
        if (factory == factories.end())
        {
            auto fmt = boost::format("Unknown entity type: %1%");
            auto msg = boost::str(fmt % demangledType);
            throw std::runtime_error(msg);
        }

        auto stackableFactoryPtr = factory->second;
        return stackableFactoryPtr;
    }
    catch (const std::bad_typeid &e)
    {
        auto fmt = boost::format("Invalid typeid operation: %1%");
        auto msg = boost::str(fmt % e.what());
        throw std::runtime_error(msg);
    }
}

PacketService::FactoryMap PacketService::StackableFactories = PacketService::FactoryMap{
    {Utility::Demangle(typeid(PacketEntity::AbsoluteEntity).name()), std::make_shared<AbsoluteFactory>()},
    {Utility::Demangle(typeid(PacketEntity::BinaryEntity).name()), std::make_shared<BinaryFactory>()},
    {Utility::Demangle(typeid(PacketEntity::EthernetEntity).name()), std::make_shared<EthernetFactory>()},
    {Utility::Demangle(typeid(PacketEntity::Ipv4Entity).name()), std::make_shared<Ipv4Factory>()},
    {Utility::Demangle(typeid(PacketEntity::UdpEntity).name()), std::make_shared<UdpFactory>()},
};
} // namespace Packet
