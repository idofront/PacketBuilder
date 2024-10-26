#include <Binary.hpp>
#include <Packet/Stackable.hpp>
#include <Utility.hpp>
#include <cstring>
#include <iostream>

namespace PluginContract
{
namespace Packet
{
Stackable::Stackable(std::size_t length, PacketEntity::StackableEntityPtr stackableEntity)
    : _Length(length), _DataArray(std::shared_ptr<Utility::DataArray>(new uint8_t[this->_Length])),
      _StackableEntity(stackableEntity), Stack(nullptr)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    std::memset(this->_DataArray.get(), 0, this->_Length);

    Stack.RegisterCallback(
        [this](StackablePtr newStackable, StackablePtr oldStackable) { OnStacked(newStackable, oldStackable); });
}

Stackable::~Stackable()
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

Utility::DataArrayPtr Stackable::DataArray() const
{
    return this->_DataArray;
}

std::size_t Stackable::Length() const
{
    return this->_Length;
}

std::size_t Stackable::GetTotalLength(StackablePtr stackable)
{
    return stackable ? stackable->Length() + GetTotalLength(stackable->Stack.Value()) : 0;
}

PacketEntity::StackableEntityPtr Stackable::StackableEntity()
{
    return this->_StackableEntity.Value();
}

void Stackable::CopyDataArray(StackablePtr stackablePtr, StackablePtr dest, std::size_t offset)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    if (stackablePtr == nullptr)
    {
        return;
    }

    auto &stackableRef = *stackablePtr;
    auto typeName = Utility::Demangle(typeid(stackableRef).name());
    auto length = stackablePtr->Length();
    SPDLOG_DEBUG("offset: {:4d}, length: {:4d}/{:4d}, typename: {}", offset, length, offset + length, typeName);

    std::memcpy(dest->DataArray().get() + offset, stackablePtr->DataArray().get(), stackablePtr->Length());
    CopyDataArray(stackablePtr->Stack.Value(), dest, offset + stackablePtr->Length());
}

std::string ToHex(uint8_t value)
{
    return std::string(1, "0123456789ABCDEF"[value >> 4]) + std::string(1, "0123456789ABCDEF"[value & 0x0F]);
}

std::string Stackable::HexDump(StackablePtr stackable)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    std::string hexDump;
    const uint8_t *data = stackable->DataArray().get();
    std::size_t length = stackable->Length();

    for (std::size_t i = 0; i < length; ++i)
    {
        hexDump += ToHex(data[i]) + " ";

        if ((i + 1) % 16 == 0)
        {
            if (i + 1 < length)
            {
                hexDump += "\n";
            }
        }
    }

    return hexDump;
}

void Stackable::OnStacked(StackablePtr oldStackable, StackablePtr newStackable)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);

    auto newStackableEntity = newStackable->StackableEntity();

    if (newStackableEntity)
    {
        this->StackableEntity()->Stack.Value(newStackableEntity);
    }
    else
    {
        SPDLOG_DEBUG("The entity of new stackable is null");
    }
}

StackablePtr Stackable::Tail(StackablePtr stackable)
{
    return stackable->Stack.Value() ? Tail(stackable->Stack.Value()) : stackable;
}
} // namespace Packet
} // namespace PluginContract
