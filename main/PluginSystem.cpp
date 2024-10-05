#include <PluginSystem.hpp>

int main(int argc, char **argv)
{
    PluginSystem::PluginSystem app;
    app.init(argc, argv);
    return app.run();
}
