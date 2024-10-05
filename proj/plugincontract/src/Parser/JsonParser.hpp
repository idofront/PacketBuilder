#ifndef PLUGINCONTRACT__PARSER__JSONPARSER_HPP
#define PLUGINCONTRACT__PARSER__JSONPARSER_HPP

#include <AbstractParser.hpp>

namespace PluginContract
{
namespace Parser
{
class JsonParser : public AbstractParser
{
  public:
    JsonParser() = default;
    virtual ~JsonParser() = default;
};
} // namespace Parser
} // namespace PluginContract

#endif
