#include <AbstractParser.hpp>
#include <Packet/PacketUtility.hpp>
#include <exception/InvalidOperationException.hpp>

namespace PluginContract
{
namespace Parser
{
AbstractParser::~AbstractParser() = default;

Packet::StackablePtr AbstractParser::Parse(Packet::StackablePtr stackable)
{
    this->_Stackable = stackable;
    auto result = this->ParseImple();

    if (result == nullptr)
    {
        throw Utility::Exception::InvalidOperationException("Could not parse the packet.");
    }

    return result;
}

const Packet::StackablePtr AbstractParser::HeadStackable()
{
    auto stackablePtr = this->Stackable();

    const auto &thisRef = *this;
    const auto &type = typeid(thisRef);
    const auto &type_name = type.name();
    auto demangled_type_name = Utility::Demangle(type_name);

    auto format = boost::format("The parser (%1%) requests the head stackable.");
    auto message = (format % demangled_type_name).str();
    SPDLOG_WARN(message);

    return stackablePtr;
}

Packet::StackablePtr AbstractParser::TailStackable()
{
    return Packet::Tail(this->Stackable());
}

Packet::StackablePtr AbstractParser::Stackable()
{
    if (this->_Stackable == nullptr)
    {
        throw Utility::Exception::InvalidOperationException("Stackable is null.");
    }

    return this->_Stackable;
}
} // namespace Parser
} // namespace PluginContract
