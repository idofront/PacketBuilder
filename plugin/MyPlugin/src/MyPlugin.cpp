#include <PluginInterface.hpp>
#include <Poco/ClassLibrary.h>

#include <MyJsonParser.hpp>
#include <Utility.hpp>
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

namespace MyPlugin
{
class MyPlugin : public PluginContract::PluginInterface
{
  public:
    MyPlugin()
    {
    }

    void ExecuteImpl() override
    {
        auto container = this->Container();
        auto jsonParser = std::make_shared<MyJsonParser>();
        container->RegisterParser(jsonParser);
        // auto msg = Utility::Demangle(typeid(*this).name());
    }
};
} // namespace MyPlugin

POCO_BEGIN_MANIFEST(PluginContract::PluginInterface)
POCO_EXPORT_CLASS(MyPlugin::MyPlugin)
POCO_END_MANIFEST