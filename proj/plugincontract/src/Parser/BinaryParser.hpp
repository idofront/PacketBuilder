#ifndef PLUGIN_CONTRACT__PARSER__BINARY_PARSER_HPP
#define PLUGIN_CONTRACT__PARSER__BINARY_PARSER_HPP

#include <Parser/AbstractParser.hpp>

namespace PluginContract
{
namespace Parser
{
class BinaryParser : public AbstractParser
{
  public:
    BinaryParser() = default;
    virtual ~BinaryParser() = default;
};
} // namespace Parser
} // namespace PluginContract

#endif
