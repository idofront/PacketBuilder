#include <Binary.hpp>
#include <Stackable.hpp>
#include <Utility.hpp>
#include <cstring>
#include <iostream>

namespace Packet
{
Stackable::Stackable(std::size_t length, PacketEntity::StackableEntityPtr stackableEntity)
    : _Length(length), _Stackable(nullptr), _DataArray(std::shared_ptr<Utility::DataArray>(new uint8_t[this->_Length])),
      _StackableEntity(stackableEntity)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    std::memset(this->_DataArray.get(), 0, this->_Length);
}

Stackable::~Stackable()
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

void Stackable::Stack(StackablePtr stackable)
{
    this->_Stackable = stackable;
    OnStacked();
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
    return stackable ? stackable->Length() + GetTotalLength(stackable->_Stackable) : 0;
}

void Stackable::CopyDataArray(StackablePtr stackable, StackablePtr dest, std::size_t offset)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    if (stackable == nullptr)
    {
        return;
    }

    auto typeName = Utility::Demangle(typeid(*stackable).name());
    auto length = stackable->Length();
    SPDLOG_DEBUG("offset: {:4d}, length: {:4d}/{:4d}, typename: {}", offset, length, offset + length, typeName);

    std::memcpy(dest->DataArray().get() + offset, stackable->DataArray().get(), stackable->Length());
    CopyDataArray(stackable->_Stackable, dest, offset + stackable->Length());
}

StackablePtr Stackable::Compose(StackablePtr stackable)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto totalLength = GetTotalLength(stackable);
    SPDLOG_DEBUG("composed data length: {}", totalLength);
    auto composed = BinaryPtr(new Binary(totalLength));
    CopyDataArray(stackable, composed);

    return composed;
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

void Stackable::OnStacked()
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

StackablePtr Stackable::Stack() const
{
    return this->_Stackable;
}

StackablePtr Stackable::Tail(StackablePtr stackable)
{
    return stackable->_Stackable ? Tail(stackable->_Stackable) : stackable;
}

} // namespace Packet
