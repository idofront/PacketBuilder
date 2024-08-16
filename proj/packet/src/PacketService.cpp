#include <PacketService.hpp>

namespace Packet
{
StackablePtr PacketService::StackableFromEntity(PacketEntity::StackableEntityPtr entity)
{
    auto type = typeid(*entity).name();
    auto demangledType = Utility::Demangle(type);

    auto typeMap = std::map<std::string, std::function<StackablePtr()>>{
        {Utility::Demangle(typeid(PacketEntity::AbsoluteEntity).name()),
         [&entity]() {
             auto entityPtr = std::dynamic_pointer_cast<PacketEntity::AbsoluteEntity>(entity);
             return std::make_shared<Absolute>(entityPtr);
         }},
        {Utility::Demangle(typeid(PacketEntity::BinaryEntity).name()),
         [&entity]() {
             auto entityPtr = std::dynamic_pointer_cast<PacketEntity::BinaryEntity>(entity);
             return std::make_shared<Binary>(entityPtr);
         }},
        {Utility::Demangle(typeid(PacketEntity::EthernetEntity).name()),
         [&entity]() {
             auto entityPtr = std::dynamic_pointer_cast<PacketEntity::EthernetEntity>(entity);
             return std::make_shared<Ethernet>(entityPtr);
         }},
        {Utility::Demangle(typeid(PacketEntity::Ipv4Entity).name()),
         [&entity]() {
             auto entityPtr = std::dynamic_pointer_cast<PacketEntity::Ipv4Entity>(entity);
             return std::make_shared<Ipv4>(entityPtr);
         }},
        {Utility::Demangle(typeid(PacketEntity::UdpEntity).name()),
         [&entity]() {
             auto entityPtr = std::dynamic_pointer_cast<PacketEntity::UdpEntity>(entity);
             return std::make_shared<Udp>(entityPtr);
         }},
    };
    auto factory = typeMap.find(demangledType);
    if (factory == typeMap.end())
    {
        auto fmt = boost::format("Unknown entity type: %1%");
        auto msg = boost::str(fmt % demangledType);
        throw std::runtime_error(msg);
    }

    auto stackable = factory->second();
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
} // namespace Packet
