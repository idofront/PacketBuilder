#include <Absolute.hpp>

namespace Packet
{
Absolute::Absolute()
    : Stackable(sizeof(this), std::make_shared<PacketEntity::AbsoluteEntity>()),
      TimestampNs(std::chrono::nanoseconds(0))
{
    RegisterCallbacks();
}

Absolute::Absolute(PacketEntity::AbsoluteEntityPtr entity)
    : Stackable(sizeof(this), entity), TimestampNs(std::chrono::nanoseconds(entity->TimestampNs))
{
    RegisterCallbacks();
}

Absolute::~Absolute()
{
}

PacketEntity::AbsoluteEntityPtr Absolute::Entity()
{
    return std::dynamic_pointer_cast<PacketEntity::AbsoluteEntity>(this->StackableEntity());
}

void Absolute::RegisterCallbacks()
{
    this->TimestampNs.RegisterCallback([this](std::chrono::nanoseconds oldValue, std::chrono::nanoseconds newValue) {
        auto timestampNs = this->TimestampNs.Value().count();

        Entity()->TimestampNs = timestampNs;
    });
}
} // namespace Packet
