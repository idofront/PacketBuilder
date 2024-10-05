#include <PluginContainer.hpp>
#include <exception/NotImplementedException.hpp>

namespace PluginContainer
{
void PluginContainer::RegisterDumper(const std::string &type, PluginContract::Dumper::DumperPtr dumper)
{
    throw Utility::Exception::NotImplementedException("Not implemented");
}

void PluginContainer::RegisterParser(const std::string &type, PluginContract::Parser::ParserPtr parser)
{
    throw Utility::Exception::NotImplementedException("Not implemented");
}
} // namespace PluginContainer
