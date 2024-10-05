// プラグインの実装例
#include <PluginInterface.hpp>
#include <Poco/ClassLibrary.h>
#include <iostream>

class MyPlugin : public Plugin::PluginInterface
{
  public:
    void execute() override
    {
        if (_dependency)
        {
            _dependency->doSomething();
        }
        std::cout << "MyPlugin executed!" << std::endl;
    }

    void setDependency(std::shared_ptr<Plugin::Dependency> dependency) override
    {
        _dependency = dependency;
    }

  private:
    std::shared_ptr<Plugin::Dependency> _dependency;
};

POCO_BEGIN_MANIFEST(Plugin::PluginInterface)
POCO_EXPORT_CLASS(MyPlugin)
POCO_END_MANIFEST
