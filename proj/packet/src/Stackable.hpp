#ifndef PACKET_BUILDER__STACKABLE_HPP__
#define PACKET_BUILDER__STACKABLE_HPP__

#include <ObservableProperty.hpp>
#include <StackableEntity.hpp>
#include <Utility.hpp>

namespace Packet
{
class Stackable;
using StackablePtr = std::shared_ptr<Stackable>;
/// @brief スタック可能なパケットを表現する．
class Stackable
{
  public:
    Stackable() = delete;
    Stackable(std::size_t length, PacketEntity::StackableEntityPtr stackableEntity);
    virtual ~Stackable() = 0;

    /// @brief データ配列を取得する．
    /// @return データ配列
    virtual Utility::DataArrayPtr DataArray() const final;

    /// @brief データ長を取得する．
    /// @return データ長
    std::size_t Length() const;

    /// @brief Stack されたデータを結合してひとまとまりのデータを取得する．
    /// @param stackable 結合するデータ
    /// @return 結合されたデータ
    static StackablePtr Compose(StackablePtr stackable);
    static std::string HexDump(StackablePtr stackable);
    static StackablePtr Tail(StackablePtr stackable);

    /// @brief スタックされるデータを設定/取得する．
    Utility::ObservableProperty<StackablePtr> Stack;

    /// @brief 自身の Entity を取得する．
    PacketEntity::StackableEntityPtr StackableEntity();

  protected:
    virtual void OnStacked(StackablePtr oldStackable, StackablePtr newStackable);
    static std::size_t GetTotalLength(StackablePtr stackable);

  private:
    std::size_t _Length;
    Utility::DataArrayPtr _DataArray;
    static void CopyDataArray(StackablePtr stackable, StackablePtr dest, std::size_t offset = 0);
    Utility::ObservableProperty<PacketEntity::StackableEntityPtr> _StackableEntity;
};
} // namespace Packet

#endif
