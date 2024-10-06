#ifndef PLUGIN__PLUGIN_HPP__
#define PLUGIN__PLUGIN_HPP__

#include <PluginContainer.hpp>
#include <Utility.hpp>
#include <memory>

namespace PluginContract
{
// インターフェース
class PluginInterface
{
  public:
    PluginInterface()
    {
        _Container = nullptr;
    }
    virtual void Execute() final
    {
        if (this->Container())
        {
            // nothing to do
        }
        else
        {
            SPDLOG_WARN("Execute plugin with null container.");
        }
        ExecuteImpl();
        SPDLOG_INFO(boost::str(boost::format("Plugin (%1%) executed.") % Name(this)));
    }
    virtual ~PluginInterface()
    {
    }

    // 依存関係を設定するメソッド
    virtual void SetContainer(PluginContract::PluginContainerPtr container) final
    {
        this->_Container = container;
    }

    static const std::string Name(PluginContract::PluginInterface *interface)
    {
        const auto &thisRef = *interface;
        const auto &type = typeid(thisRef);
        const auto &type_name = type.name();
        auto demangled_type_name = Utility::Demangle(type_name);
        return demangled_type_name;
    }

  protected:
    virtual const PluginContract::PluginContainerPtr &Container() const final
    {
        return this->_Container;
    }
    virtual void ExecuteImpl() = 0;

  private:
    PluginContract::PluginContainerPtr _Container;
};

} // namespace PluginContract

#endif
