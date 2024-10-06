#ifndef PLUGINCONTRACT__PARSER__PCAPPARSER_HPP
#define PLUGINCONTRACT__PARSER__PCAPPARSER_HPP

#include <AbstractParser.hpp>

namespace PluginContract
{
namespace Parser
{
class PcapParser : public AbstractParser
{
  public:
    PcapParser() = default;
    virtual ~PcapParser() = default;
};
} // namespace Parser
} // namespace PluginContract

#endif
