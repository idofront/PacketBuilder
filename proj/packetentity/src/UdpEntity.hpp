#ifndef PACKET_ENTITY__UDP_ENTITY_HPP
#define PACKET_ENTITY__UDP_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class UdpEntity;
using UdpEntityPtr = std::shared_ptr<UdpEntity>;
/// @brief Udp ヘッダを表現する．
class UdpEntity : public StackableEntity
{
  public:
    UdpEntity();
    virtual ~UdpEntity();
    virtual nlohmann::json ToJson() override;
    static StackableEntityPtr FromJson(nlohmann::json json);

  public:
    /// @brief 発信元ポート番号
    uint16_t SourcePort;

    /// @brief 宛先ポート番号
    uint16_t DestinationPort;

    /// @brief 長さ
    uint16_t Length;

    /// @brief チェックサム
    uint16_t Checksum;
};
} // namespace PacketEntity

#endif
