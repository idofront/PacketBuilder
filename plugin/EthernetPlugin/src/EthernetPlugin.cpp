#include <PluginInterface.hpp>
#include <Poco/ClassLibrary.h>

#include <spdlog/spdlog.h>

namespace EthernetPlugin
{
class EthernetPlugin : public PluginContract::PluginInterface
{
  public:
    EthernetPlugin()
    {
    }

    void ExecuteImpl() override
    {
        auto container = this->Container();
    }
};
} // namespace EthernetPlugin

POCO_BEGIN_MANIFEST(PluginContract::PluginInterface)
POCO_EXPORT_CLASS(EthernetPlugin::EthernetPlugin)
POCO_END_MANIFEST
