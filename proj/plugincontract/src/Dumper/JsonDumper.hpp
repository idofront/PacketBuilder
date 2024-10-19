#ifndef PLUGINCONTRACT__DUMPER__JSONDUMPER_HPP
#define PLUGINCONTRACT__DUMPER__JSONDUMPER_HPP

#include "AbstractDumper.hpp"

namespace PluginContract
{
namespace Dumper
{
class JsonDumper : public AbstractDumper
{
  public:
    JsonDumper() = default;
    virtual ~JsonDumper() = default;
};
} // namespace Dumper
} // namespace PluginContract

#endif
