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

    auto stackedEntity = entity->Stack.Value();
    if (stackedEntity)
    {
        auto stackedType = typeid(*stackedEntity).name();
        auto demangledStackedType = Utility::Demangle(stackedType);
        auto fmt = boost::format("The entity has a stack: %1%");
        auto msg = boost::str(fmt % demangledStackedType);
        SPDLOG_DEBUG(msg);

        auto stackedStackable = StackableFromEntity(stackedEntity);
        stackedType = typeid(*stackedStackable).name();
        demangledStackedType = Utility::Demangle(stackedType);
        fmt = boost::format("Try to stack the entity: %1%");
        msg = boost::str(fmt % demangledStackedType);
        SPDLOG_DEBUG(msg);

        stackable->Stack.Value(stackedStackable);
    }

    return stackable;
}

StackableFactoryPtr PacketService::GetStackableFactory(PacketEntity::StackableEntityPtr entity)
{
    auto type = typeid(*entity).name();
    auto demangledType = Utility::Demangle(type);

    auto factory = StackableFactories.find(demangledType);
    if (factory == StackableFactories.end())
    {
        auto fmt = boost::format("Unknown entity type: %1%");
        auto msg = boost::str(fmt % demangledType);
        throw std::runtime_error(msg);
    }

    auto stackableFactoryPtr = factory->second;
    return stackableFactoryPtr;
}

std::map<std::string, StackableFactoryPtr> PacketService::StackableFactories = {
    {Utility::Demangle(typeid(PacketEntity::AbsoluteEntity).name()), std::make_shared<AbsoluteFactory>()},
    {Utility::Demangle(typeid(PacketEntity::BinaryEntity).name()), std::make_shared<BinaryFactory>()},
    {Utility::Demangle(typeid(PacketEntity::EthernetEntity).name()), std::make_shared<EthernetFactory>()},
    {Utility::Demangle(typeid(PacketEntity::Ipv4Entity).name()), std::make_shared<Ipv4Factory>()},
    {Utility::Demangle(typeid(PacketEntity::UdpEntity).name()), std::make_shared<UdpFactory>()}};
} // namespace Packet
