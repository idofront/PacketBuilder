#ifndef PACKET_ENTITY__STACKABLE_ENTITY_HPP
#define PACKET_ENTITY__STACKABLE_ENTITY_HPP

#include <ObservableProperty.hpp>
#include <boost/format.hpp>
#include <memory>
#include <nlohmann/json.hpp>

namespace PacketEntity
{
class StackableEntity;
using StackableEntityPtr = std::shared_ptr<StackableEntity>;
/// @brief スタック可能なエンティティを表現する．
class StackableEntity
{
  public:
    StackableEntity();
    virtual ~StackableEntity();
    Utility::ObservableProperty<StackableEntityPtr> Stack;

    virtual std::string ToString();
    virtual nlohmann::json ToJson() = 0;

  private:
    virtual std::string EntityType() final;
};
} // namespace PacketEntity

#endif
