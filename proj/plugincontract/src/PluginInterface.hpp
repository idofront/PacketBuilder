#ifndef PLUGIN__PLUGIN_HPP__
#define PLUGIN__PLUGIN_HPP__

#include <memory>

namespace PluginContract
{
// 依存関係の例としてのコンポーネント
class Dependency
{
  public:
    void doSomething() const
    {
    }
};

// インターフェース
class PluginInterface
{
  public:
    virtual void execute() = 0;
    virtual ~PluginInterface()
    {
    }

    // 依存関係を設定するメソッド
    virtual void setDependency(std::shared_ptr<Dependency> dependency) = 0;
};

} // namespace PluginContract

#endif
