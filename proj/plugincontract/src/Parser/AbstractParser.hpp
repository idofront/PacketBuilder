#ifndef PLUGINCONTRACT__PARSER__ABSTRACTPARSER_HPP
#define PLUGINCONTRACT__PARSER__ABSTRACTPARSER_HPP

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
};

using ParserPtr = std::shared_ptr<Parser::AbstractParser>;
} // namespace Parser
} // namespace PluginContract

#endif
