#ifndef MY_PLUGIN__MY_JSON_PARSER_HPP
#define MY_PLUGIN__MY_JSON_PARSER_HPP

#include <Dumper/JsonDumper.hpp>

namespace MyPlugin
{
class MyJsonDumpler : public PluginContract::Dumper::JsonDumper
{
};
} // namespace MyPlugin
#endif
