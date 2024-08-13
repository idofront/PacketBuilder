#ifndef PACKET_ENTITY__STACKABLE_ENTITY_HPP
#define PACKET_ENTITY__STACKABLE_ENTITY_HPP

#include <boost/format.hpp>
#include <memory>
#include <nlohmann/json.hpp>

namespace PacketEntity
{
class StackableEntity;
using StackableEntityPtr = std::shared_ptr<StackableEntity>;

class StackableEntity
{
  public:
    StackableEntity();
    virtual ~StackableEntity();
    StackableEntityPtr Stack();

    virtual std::string ToString();
    virtual nlohmann::json ToJson() = 0;

  private:
    StackableEntityPtr _Stack;
};
} // namespace PacketEntity

#endif