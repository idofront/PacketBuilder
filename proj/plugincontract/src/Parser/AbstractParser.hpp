#ifndef PLUGINCONTRACT__PARSER__ABSTRACTPARSER_HPP
#define PLUGINCONTRACT__PARSER__ABSTRACTPARSER_HPP

#include <Packet/Stackable.hpp>
#include <memory>

namespace PluginContract
{
namespace Parser
{
class AbstractParser
{
  public:
    AbstractParser() = default;
    virtual ~AbstractParser() = 0;

    /// @brief パケットを解析する．
    /// @param stackable
    /// @return
    virtual Packet::StackablePtr Parse(Packet::StackablePtr stackable) final;

  protected:
    /// @brief Stackable の先頭から取得する．
    /// @return Stackable
    /// @details 末尾以外の Stackable は利用しないことを推奨する．
    virtual const Packet::StackablePtr HeadStackable() final;

    /// @brief Stackable の末尾を取得する．
    /// @return Stackable
    virtual Packet::StackablePtr TailStackable() final;

    /// @brief 解析の前処理を実装する．
    virtual void PreParseImple();

    /// @brief 解析を実装する．
    /// @return 解析結果として生成した Stackable を返す．
    /// @details 解析結果を Stack せず，また解析した残りのデータも解析結果に Stack しないこと．これらは PostParseImple
    /// で行う．
    virtual Packet::StackablePtr ParseImple() = 0;

    /// @brief 解析の後処理を実装する．
    /// @param ParseImple の解析結果
    /// @return Stackable の先頭から全データを返す．
    /// @details PostParseImple は override しないことを推奨する．
    virtual Packet::StackablePtr PostParseImple(Packet::StackablePtr);

  private:
    Packet::StackablePtr _Stackable;
    Packet::StackablePtr Stackable();
};

using ParserPtr = std::shared_ptr<Parser::AbstractParser>;
} // namespace Parser
} // namespace PluginContract

#endif
