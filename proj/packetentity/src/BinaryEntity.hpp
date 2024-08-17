#ifndef PACKET_ENTITY__BINARY_ENTITY_HPP
#define PACKET_ENTITY__BINARY_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
using DataArray = std::vector<uint8_t>;
using DataArrayPtr = std::shared_ptr<DataArray>;

class BinaryEntity;
using BinaryEntityPtr = std::shared_ptr<BinaryEntity>;
/// @brief バイナリデータを表現する．
/// @details 主にペイロード部を表現する．
class BinaryEntity : public StackableEntity
{
  public:
    BinaryEntity(std::size_t length);
    virtual ~BinaryEntity();
    virtual nlohmann::json ToJson() override;
    static StackableEntityPtr FromJson(nlohmann::json json);

  public:
    /// @brief データ配列
    DataArrayPtr Data;
};
} // namespace PacketEntity

#endif
