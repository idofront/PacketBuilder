#ifndef PACKET_ENTITY__ABSOLUTE_ENTITY_HPP
#define PACKET_ENTITY__ABSOLUTE_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class AbsoluteEntity;
using AbsoluteEntityPtr = std::shared_ptr<AbsoluteEntity>;
/// @brief キャプチャの絶対時間を表現する．
class AbsoluteEntity : public StackableEntity
{
  public:
    AbsoluteEntity();
    virtual ~AbsoluteEntity();
    virtual nlohmann::json ToJson() override;
    static StackableEntityPtr FromJson(nlohmann::json json);

  public:
    /// @brief タイムスタンプ
    uint64_t TimestampNs;
};

} // namespace PacketEntity

#endif
