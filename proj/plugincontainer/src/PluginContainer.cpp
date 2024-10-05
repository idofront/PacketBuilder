#include <PluginContainer.hpp>
#include <exception/NotImplementedException.hpp>

#include <Utility.hpp>
#include <boost/format.hpp>
#include <spdlog/spdlog.h>

namespace PluginContainer
{
void PluginContainer::RegisterDumper(PluginContract::Dumper::DumperPtr dumper)
{
    if (!dumper)
    {
        throw std::invalid_argument("Dumper is null.");
    }

    auto &dumperRef = *dumper;
    const auto &type = typeid(dumperRef);
    auto type_name = Utility::Demangle(type.name());

    auto dumperPtr = this->ResolveDumper<PluginContract::Dumper::AbstractDumper>();

    if (dumperPtr != nullptr)
    {
        SPDLOG_WARN(boost::str(boost::format("Dumper {} already registered.") % type_name));
    }
    SPDLOG_INFO(boost::str(boost::format("Registered dumper: {}") % type_name));
    _Dumpers.push_back(dumper);
}

void PluginContainer::RegisterParser(PluginContract::Parser::ParserPtr parser)
{
    if (!parser)
    {
        throw std::invalid_argument("Parser is null.");
    }

    auto &parserRef = *parser;
    const auto &type = typeid(parserRef);
    auto type_name = Utility::Demangle(type.name());

    auto parserPtr = this->ResolveParser<PluginContract::Parser::AbstractParser>();

    if (parserPtr != nullptr)
    {
        SPDLOG_WARN(boost::str(boost::format("Parser {} already registered.") % type_name));
    }
    SPDLOG_INFO(boost::str(boost::format("Registered parser: {}") % type_name));
    _Parsers.push_back(parser);
}
} // namespace PluginContainer
