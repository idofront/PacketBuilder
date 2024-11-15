#ifndef PLUGIN_SYSTEM_HPP__
#define PLUGIN_SYSTEM_HPP__

#include <Packet/Binary.hpp>
#include <PluginInterface.hpp>
#include <PluginPacketBuilder.hpp>
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

/// @brief 動作確認用のダミー Stackable を生成する．
/// @return Stackable
/// @details 実装途中に動作確認するための Stackable であり，将来的に削除される．
PluginContract::Packet::StackablePtr GenerateDummyStackable()
{
    auto ethernetRawData =
        std::vector<uint8_t>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x80, 0x00};
    auto ipRawData = std::vector<uint8_t>{0x45, 0x00, 0x00, 0x3C, 0x00, 0x01, 0x00, 0x00, 0x40, 0x01,
                                          0x00, 0x00, 0x0A, 0x00, 0x00, 0x01, 0x0A, 0x00, 0x00, 0x02};

    auto totalLength = ethernetRawData.size() + ipRawData.size();
    auto binary = std::make_shared<PluginContract::Packet::Binary>(totalLength);

    auto data = binary->DataArray().get();
    std::copy(ethernetRawData.begin(), ethernetRawData.end(), data);
    std::copy(ipRawData.begin(), ipRawData.end(), data + ethernetRawData.size());

    return binary;
}

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

            auto pluginPacketBuilder = std::make_shared<PluginPacketBuilder::PluginPacketBuilder>(_Container);
            auto plugins = std::vector<std::shared_ptr<PluginContract::PluginInterface>>();

            for (; it != end; ++it)
            {
                if (it->isFile() && (it.path().getExtension() == "so" || it.path().getExtension() == "dll"))
                {
                    std::string pluginPath = it.path().toString();
                    try
                    {
                        loader.loadLibrary(pluginPath);
                    }
                    catch (const std::exception &e)
                    {
                        auto fileName = it.path().getFileName();
                        auto fmt = boost::format("Failed to load plugin library: %1% (%2%)");
                        auto msg = fmt % fileName % e.what();
                        SPDLOG_ERROR(msg.str());
                        continue;
                    }

                    SPDLOG_INFO("Loaded plugin library: " + pluginPath);

                    // マニフェストからプラグインを取得
                    PluginLoader::Iterator loaderIt = loader.begin();
                    PluginLoader::Iterator loaderEnd = loader.end();
                    for (; loaderIt != loaderEnd; ++loaderIt)
                    {
                        PluginManifest::Iterator manifestIt = loaderIt->second->begin();
                        PluginManifest::Iterator manifestEnd = loaderIt->second->end();
                        for (; manifestIt != manifestEnd; ++manifestIt)
                        {
                            auto plugin = std::shared_ptr<PluginContract::PluginInterface>(manifestIt->create());
                            plugin->SetContainer(_Container);
                            plugin->Execute();
                            plugins.push_back(plugin);
                        }
                    }
                }
                else
                {
                    logger().information("Skipped: " + it.path().toString());
                }
            }

            // ダミー Stackable を生成
            auto dummyStackable = GenerateDummyStackable();

            // TODO ダミー Stackable を解析する
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
