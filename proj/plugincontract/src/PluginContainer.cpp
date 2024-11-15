#include <PluginContainer.hpp>
#include <exception/NotImplementedException.hpp>

#include <Utility.hpp>
#include <boost/format.hpp>
#include <spdlog/spdlog.h>

namespace PluginContract
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

    SPDLOG_INFO(boost::str(boost::format("Registered dumper: %1%") % type_name));
    _Dumpers.push_back(dumper);
}

void PluginContainer::RegisterParser(ParserPtr parser, ParserCondition condition)
{
    if (!parser)
    {
        throw std::invalid_argument("Parser is null.");
    }

    auto &parserRef = *parser;
    const auto &type = typeid(parserRef);
    auto type_name = Utility::Demangle(type.name());

    SPDLOG_INFO(boost::str(boost::format("Registered parser: %1%") % type_name));

    _Parsers.push_back({parser, condition});
}
} // namespace PluginContract
