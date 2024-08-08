#ifndef JSON_CONVERTER__STACKABLE__ENTITY_HPP__
#define JSON_CONVERTER__STACKABLE__ENTITY_HPP__

#include <nlohmann/json.hpp>
#include <string>

namespace JsonConverter
{
class StackableEntity;
using StackableEntityPtr = std::shared_ptr<StackableEntity>;
class StackableEntity
{
  public:
    StackableEntity();
    virtual ~StackableEntity();

    /// @brief Stackable.Stack の型
    /// Stack がない場合は空文字列
    std::string StackType;

    StackableEntityPtr Stack;
};
} // namespace JsonConverter
#endif
