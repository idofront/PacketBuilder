#ifndef PACKET_BUILDER__STACKABLE_HPP__
#define PACKET_BUILDER__STACKABLE_HPP__

#include <NotifyProperty.hpp>
#include <StackableEntity.hpp>
#include <Utility.hpp>

namespace Packet
{
class Stackable;

using StackablePtr = std::shared_ptr<Stackable>;

class Stackable
{
  public:
    Stackable() = delete;
    Stackable(std::size_t length, PacketEntity::StackableEntityPtr stackableEntity);
    virtual ~Stackable() = 0;
    virtual Utility::DataArrayPtr DataArray() const final;
    std::size_t Length() const;

    static StackablePtr Compose(StackablePtr stackable);
    static std::string HexDump(StackablePtr stackable);
    static StackablePtr Tail(StackablePtr stackable);

    Utility::NotifyProperty<StackablePtr> Stack;

    PacketEntity::StackableEntityPtr StackableEntity();

  protected:
    virtual void OnStacked(StackablePtr newStackable, StackablePtr oldStackable);
    static std::size_t GetTotalLength(StackablePtr stackable);

  private:
    std::size_t _Length;
    Utility::DataArrayPtr _DataArray;
    static void CopyDataArray(StackablePtr stackable, StackablePtr dest, std::size_t offset = 0);
    PacketEntity::StackableEntityPtr _StackableEntity;
};
} // namespace Packet

#endif
