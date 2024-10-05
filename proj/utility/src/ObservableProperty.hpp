#ifndef UTILITY__NOTIFY_PROPERTY_HPP
#define UTILITY__NOTIFY_PROPERTY_HPP

#include <memory>
#include <functional>

namespace Utility
{
template <typename T> class ObservableProperty;
template <typename T> using ObservablePropertyPtr = std::shared_ptr<ObservableProperty<T>>;

/// @brief 値の変更通知を提供可能なプロパティ
/// @tparam T 値の型
/// @details 値が変化したかどうかによらず Value を Set されると Notify が呼ばれる．
template <typename T> class ObservableProperty
{
  public:
    ObservableProperty() = delete;
    ObservableProperty(T value) : _Value(value)
    {
    }

    T Value() const
    {
        return _Value;
    }

    void Value(T value)
    {
        auto oldValue = _Value;
        _Value = value;
        Notify(oldValue, value);
    }

    /// @brief 値が変更されたときに呼ばれるコールバックを登録する．
    /// @param callback コールバック
    void RegisterCallback(std::function<void(T, T)> callback)
    {
        _Callbacks.push_back(callback);
    }

  private:
    T _Value;
    std::vector<std::function<void(T, T)>> _Callbacks;
    void Notify(T oldValue, T newValue)
    {
        for (auto callback : _Callbacks)
        {
            callback(oldValue, newValue);
        }
    }
};
} // namespace Utility

#endif
