#ifndef PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__
#define PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__

#include <Dumper/AbstractDumper.hpp>
#include <Parser/AbstractParser.hpp>
#include <memory>
#include <vector>

namespace PluginContract
{
class PluginContainer
{
  public:
    PluginContainer() = default;
    virtual ~PluginContainer() = default;

    void RegisterDumper(PluginContract::Dumper::DumperPtr dumper);
    template <class T> PluginContract::Dumper::DumperPtr ResolveDumper()
    {
        auto dumperItr = std::find_if(_Dumpers.begin(), _Dumpers.end(), [](PluginContract::Dumper::DumperPtr dumper) {
            return dynamic_cast<T *>(dumper.get()) != nullptr;
        });
        return dumperItr != _Dumpers.end() ? *dumperItr : nullptr;
    }

    void RegisterParser(PluginContract::Parser::ParserPtr parser);
    template <class T> PluginContract::Parser::ParserPtr ResolveParser()
    {
        auto parserItr = std::find_if(_Parsers.begin(), _Parsers.end(), [](PluginContract::Parser::ParserPtr parser) {
            return dynamic_cast<T *>(parser.get()) != nullptr;
        });
        return parserItr != _Parsers.end() ? *parserItr : nullptr;
    }

  private:
    std::vector<PluginContract::Dumper::DumperPtr> _Dumpers;
    std::vector<PluginContract::Parser::ParserPtr> _Parsers;
};

using PluginContainerPtr = std::shared_ptr<PluginContainer>;
} // namespace PluginContainer

#endif
