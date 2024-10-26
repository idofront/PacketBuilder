#ifndef PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__
#define PLUGIN_CONTAINER__PLUGIN_CONTAINER_HPP__

#include <Dumper/AbstractDumper.hpp>
#include <Packet/Stackable.hpp>
#include <Parser/AbstractParser.hpp>
#include <exception/NotImplementedException.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace PluginContract
{
class PluginContainer
{
  public:
    using ParserPtr = PluginContract::Parser::ParserPtr;
    using ParserCondition = std::function<bool(Packet::StackablePtr)>;
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

    /// @brief 指定した型の Parser を取得する．
    /// @tparam T 取得する Parser の型
    /// @return Parser のインスタンス．見つからない場合は nullptr
    template <class T> ParserPtr ResolveParser()
    {
        auto parserItr = std::find_if(_Parsers.begin(), _Parsers.end(), [this](ParserTuple parserTuple) {
            auto parser = ParserPtr();
            auto condition = ParserCondition();
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
            auto condition = ParserCondition();
            SplitParserTuple(*parserItr, parser, condition);
            return parser;
        }
    }

    /// @brief 指定したパケットに対して適用可能な Parser を取得する．
    /// @param ptr 対象のパケット
    /// @return 適用可能な Parser のリスト
    std::vector<ParserPtr> FilterParsers(const Packet::StackablePtr &ptr)
    {
        auto parserTuples = std::vector<ParserTuple>();

        // filter にマッチしたものだけを取り出す．
        std::copy_if(_Parsers.begin(), _Parsers.end(), std::back_inserter(parserTuples),
                     [ptr](ParserTuple parserTuple) {
                         auto parser = ParserPtr();
                         auto condition = ParserCondition();
                         SplitParserTuple(parserTuple, parser, condition);
                         return condition(ptr);
                     });

        auto parserPtrs = std::vector<ParserPtr>();

        std::transform(parserTuples.begin(), parserTuples.end(), std::back_inserter(parserPtrs),
                       [](ParserTuple parserTuple) {
                           auto parser = ParserPtr();
                           auto condition = ParserCondition();
                           SplitParserTuple(parserTuple, parser, condition);
                           return parser;
                       });

        return parserPtrs;
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
