#ifndef PLUGINCONTRACT__DUMPER__ABSTRACTDUMPER_HPP
#define PLUGINCONTRACT__DUMPER__ABSTRACTDUMPER_HPP

namespace PluginContract
{
namespace Dumper
{
class AbstractDumper
{
  public:
    AbstractDumper() = default;
    virtual ~AbstractDumper() = 0;
};
} // namespace Dumper
} // namespace PluginContract

#endif
