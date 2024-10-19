#ifndef PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__
#define PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__

#include <Dumper/AbstractDumper.hpp>
#include <Parser/AbstractParser.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace PluginContract
{
class PluginContainer
{
  public:
    using ParserPtr = PluginContract::Parser::ParserPtr;
    using ParserCondition = std::function<bool()>;
    using ParserTuple = std::tuple<ParserPtr, ParserCondition>;

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

    void RegisterParser(ParserPtr parser, ParserCondition condition);
    template <class T> ParserPtr ResolveParser()
    {
        auto parserItr = std::find_if(_Parsers.begin(), _Parsers.end(), [this](ParserTuple parserTuple) {
            auto parser = ParserPtr();
            auto condition = std::function<bool()>();
            SplitParserTuple(parserTuple, parser, condition);
            return dynamic_cast<T *>(parser.get()) != nullptr;
        });

        if (parserItr == _Parsers.end())
        {
            return nullptr;
        }
        else
        {
            auto parser = ParserPtr();
            auto condition = std::function<bool()>();
            SplitParserTuple(*parserItr, parser, condition);
            return parser;
        }
    }

  private:
    static void SplitParserTuple(ParserTuple &parserTuple, ParserPtr &parser, ParserCondition &condition)
    {
        parser = std::get<0>(parserTuple);
        condition = std::get<1>(parserTuple);
    }
    std::vector<PluginContract::Dumper::DumperPtr> _Dumpers;
    std::vector<ParserTuple> _Parsers;
};

using PluginContainerPtr = std::shared_ptr<PluginContainer>;
} // namespace PluginContract

#endif
