#include <Binary.hpp>
#include <Packet/Stackable.hpp>
#include <Utility.hpp>
#include <cstring>
#include <iostream>

namespace PluginContract
{
namespace Packet
{
Stackable::Stackable(std::size_t length)
    : _Length(length), _DataArray(std::shared_ptr<Utility::DataArray>(new uint8_t[this->_Length])), Stack(nullptr)
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

void Stackable::OnStacked(StackablePtr oldStackable, StackablePtr newStackable)
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
}

} // namespace Packet
} // namespace PluginContract
