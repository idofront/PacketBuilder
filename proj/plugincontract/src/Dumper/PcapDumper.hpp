#ifndef PLUGINCONTRACT__DUMPER__PCAPDUMPER_HPP
#define PLUGINCONTRACT__DUMPER__PCAPDUMPER_HPP

#include "AbstractDumper.hpp"

namespace PluginContract
{
namespace Dumper
{
class PcapDumper : public AbstractDumper
{
  public:
    PcapDumper() = default;
    virtual ~PcapDumper() = default;
};
} // namespace Dumper
} // namespace PluginContract

#endif
