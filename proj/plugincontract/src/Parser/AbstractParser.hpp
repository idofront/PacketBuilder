#ifndef PLUGINCONTRACT__PARSER__ABSTRACTPARSER_HPP
#define PLUGINCONTRACT__PARSER__ABSTRACTPARSER_HPP

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
} // namespace Parser
} // namespace PluginContract

#endif
