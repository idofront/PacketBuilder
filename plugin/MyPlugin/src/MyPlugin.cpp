// プラグインの実装例
#include <PluginInterface.hpp>
#include <Poco/ClassLibrary.h>
#include <iostream>

class MyPlugin : public PluginContract::PluginInterface
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

    void setDependency(std::shared_ptr<PluginContract::Dependency> dependency) override
    {
        _dependency = dependency;
    }

  private:
    std::shared_ptr<PluginContract::Dependency> _dependency;
};

POCO_BEGIN_MANIFEST(PluginContract::PluginInterface)
POCO_EXPORT_CLASS(MyPlugin)
POCO_END_MANIFEST
