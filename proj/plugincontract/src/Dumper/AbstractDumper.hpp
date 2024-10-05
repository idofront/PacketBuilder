#ifndef PLUGINCONTRACT__DUMPER__ABSTRACTDUMPER_HPP
#define PLUGINCONTRACT__DUMPER__ABSTRACTDUMPER_HPP

#include <memory>

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

using DumperPtr = std::shared_ptr<Dumper::AbstractDumper>;
} // namespace Dumper
} // namespace PluginContract

#endif
