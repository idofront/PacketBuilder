#include <PluginSystem.hpp>
#include <PluginContainer.hpp>

int main(int argc, char **argv)
{
    PluginContract::PluginContainerPtr container = std::make_shared<PluginContract::PluginContainer>();
    PluginSystem::PluginSystem app;
    app.init(argc, argv);
    return app.run();
}
