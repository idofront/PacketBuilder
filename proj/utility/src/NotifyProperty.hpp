#ifndef UTILITY__NOTIFY_PROPERTY_HPP
#define UTILITY__NOTIFY_PROPERTY_HPP

#include <functional>

namespace Utility
{
template <typename T> class NotifyProperty
{
  public:
    NotifyProperty() = delete;
    NotifyProperty(T value) : _Value(value)
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
