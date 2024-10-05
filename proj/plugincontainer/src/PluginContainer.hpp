#ifndef PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__
#define PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__

#include <Dumper/AbstractDumper.hpp>
#include <Parser/AbstractParser.hpp>
#include <memory>

namespace PluginContainer
{
class PluginContainer
{
  public:
    PluginContainer() = default;
    virtual ~PluginContainer() = default;

    void RegisterDumper(const std::string &type, PluginContract::Dumper::DumperPtr dumper);
    void RegisterParser(const std::string &type, PluginContract::Parser::ParserPtr parser);
};

using PluginContainerPtr = std::shared_ptr<PluginContainer>;
} // namespace PluginContainer

#endif
