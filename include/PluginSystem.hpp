#ifndef PLUGIN_SYSTEM_HPP__
#define PLUGIN_SYSTEM_HPP__

#include <PluginInterface.hpp>
#include <Poco/ClassLoader.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/Manifest.h>
#include <Poco/Path.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/OptionSet.h>
#include <iostream>
#include <memory>

using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;

namespace PluginSystem
{
typedef Poco::ClassLoader<PluginContract::PluginInterface> PluginLoader;
typedef Poco::Manifest<PluginContract::PluginInterface> PluginManifest;

class PluginSystem : public Application
{
  protected:
    void initialize(Application &self) override
    {
        Application::initialize(self);
        logger().information("Initializing");

        this->_Container = std::make_shared<PluginContract::PluginContainer>();
    }

    void uninitialize() override
    {
        logger().information("Shutting down");
        Application::uninitialize();
    }

    int main(const ArgVec &args) override
    {
        Poco::Path basePath(config().getString("application.path"));
        std::string pluginDir = config().getString("pluginDir", basePath.parent().toString() + "/plugins");
        PluginLoader loader;

        try
        {
            Poco::Path pluginDirPath(pluginDir);
            Poco::DirectoryIterator it(pluginDirPath);
            Poco::DirectoryIterator end;
            for (; it != end; ++it)
            {
                if (it->isFile() && (it.path().getExtension() == "so" || it.path().getExtension() == "dll"))
                {
                    std::string pluginPath = it.path().toString();
                    loader.loadLibrary(pluginPath);
                    logger().information("Loaded plugin library: " + pluginPath);

                    // マニフェストからプラグインを取得
                    PluginLoader::Iterator loaderIt = loader.begin();
                    PluginLoader::Iterator loaderEnd = loader.end();
                    for (; loaderIt != loaderEnd; ++loaderIt)
                    {
                        PluginManifest::Iterator manifestIt = loaderIt->second->begin();
                        PluginManifest::Iterator manifestEnd = loaderIt->second->end();
                        for (; manifestIt != manifestEnd; ++manifestIt)
                        {
                            PluginContract::PluginInterface *plugin = manifestIt->create();
                            plugin->SetContainer(_Container);
                            plugin->Execute();
                            delete plugin;
                        }
                    }
                }
                else
                {
                    logger().information("Skipped: " + it.path().toString());
                }
            }
        }
        catch (Poco::Exception &ex)
        {
            logger().error("Error: " + ex.displayText());
            return Application::EXIT_SOFTWARE;
        }

        return Application::EXIT_OK;
    }

    void defineOptions(OptionSet &options) override
    {
        Application::defineOptions(options);

        options.addOption(Option("help", "h", "display help information")
                              .required(false)
                              .repeatable(false)
                              .callback(OptionCallback<PluginSystem>(this, &PluginSystem::handleHelp)));

        options.addOption(Option("pluginDir", "p", "specify the directory to load plugins from")
                              .required(false)
                              .repeatable(false)
                              .argument("directory")
                              .binding("pluginDir"));
    }

    void handleHelp(const std::string &name, const std::string &value)
    {
        HelpFormatter helpFormatter(options());
        helpFormatter.format(std::cout);
        stopOptionsProcessing();
    }

  private:
    PluginContract::PluginContainerPtr _Container;
};
} // namespace PluginSystem

#endif
