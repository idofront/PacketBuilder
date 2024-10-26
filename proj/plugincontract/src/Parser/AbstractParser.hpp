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
    virtual Packet::StackablePtr TailStackable() final;
    virtual Packet::StackablePtr ParseImple() = 0;

  private:
    Packet::StackablePtr _Stackable;
    Packet::StackablePtr Stackable();
};

using ParserPtr = std::shared_ptr<Parser::AbstractParser>;
} // namespace Parser
} // namespace PluginContract

#endif
