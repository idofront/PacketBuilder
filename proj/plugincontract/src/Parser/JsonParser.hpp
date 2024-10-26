#ifndef PLUGINCONTRACT__PARSER__JSONPARSER_HPP
#define PLUGINCONTRACT__PARSER__JSONPARSER_HPP

#include <Parser/AbstractParser.hpp>
#include <spdlog/spdlog.h>

namespace PluginContract
{
namespace Parser
{
class JsonParser : public AbstractParser
{
  public:
    JsonParser() = default;
    virtual ~JsonParser() = default;
    virtual Packet::StackablePtr ParseImple() override
    {
        throw Utility::Exception::NotImplementedException("Not implemented.");
    }
};
} // namespace Parser
} // namespace PluginContract

#endif
