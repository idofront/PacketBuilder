#ifndef UTILITY__READ_ONLY_OBSERVABLE_PROPERTY_HPP
#define UTILITY__READ_ONLY_OBSERVABLE_PROPERTY_HPP

#include <ObservableProperty.hpp>

namespace Utility
{
template <typename T> class ReadOnlyObservableProperty;
template <typename T> using ReadOnlyObservablePropertyPtr = std::shared_ptr<ReadOnlyObservableProperty<T>>;

/// @brief 値の変更通知を提供可能な読み取り専用のプロパティ
/// @tparam T 値の型
/// @details コンストラクタで ObservableProperty を受け取り，そのインスタンスの変化を通知する．
template <typename T> class ReadOnlyObservableProperty
{
  public:
    ReadOnlyObservableProperty() = delete;
    ReadOnlyObservableProperty(ObservablePropertyPtr<T> propertyPtr)
    {
        if (propertyPtr == nullptr)
        {
            throw std::runtime_error("Null pointer is not allowed.");
        }

        _PropertyPtr = propertyPtr;
    }

    T Value() const
    {
        return _PropertyPtr->Value();
    }

    /// @brief 値が変更されたときに呼ばれるコールバックを登録する．
    /// @param callback コールバック
    void RegisterCallback(std::function<void(T, T)> callback)
    {
        _PropertyPtr->RegisterCallback(callback);
    }

  private:
    ObservablePropertyPtr<T> _PropertyPtr;
};
} // namespace Utility

#endif
