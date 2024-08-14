#ifndef PACKET_BUILDER__STACKABLE_HPP__
#define PACKET_BUILDER__STACKABLE_HPP__

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
    virtual void Stack(StackablePtr stackable) final;
    virtual Utility::DataArrayPtr DataArray() const final;
    std::size_t Length() const;

    static StackablePtr Compose(StackablePtr stackable);
    static std::string HexDump(StackablePtr stackable);
    static StackablePtr Tail(StackablePtr stackable);
    StackablePtr Stack() const;

  protected:
    virtual void OnStacked();
    static std::size_t GetTotalLength(StackablePtr stackable);
    PacketEntity::StackableEntityPtr StackableEntity();

  private:
    StackablePtr _Stackable = nullptr;
    std::size_t _Length;
    Utility::DataArrayPtr _DataArray;
    static void CopyDataArray(StackablePtr stackable, StackablePtr dest, std::size_t offset = 0);
    PacketEntity::StackableEntityPtr _StackableEntity;
};
} // namespace Packet

#endif
