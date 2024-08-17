#ifndef PACKET_ENTITY__PCAP_PACKET_HEADER_ENTITY_HPP
#define PACKET_ENTITY__PCAP_PACKET_HEADER_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class PcapPacketHeaderEntity;
using PcapPacketHeaderEntityPtr = std::shared_ptr<PcapPacketHeaderEntity>;
/// @brief Pcap パケットヘッダを表現する．
class PcapPacketHeaderEntity : public StackableEntity
{
  public:
    PcapPacketHeaderEntity();
    virtual ~PcapPacketHeaderEntity();
    virtual nlohmann::json ToJson() override;

  public:
    /// @brief タイムスタンプ（秒）
    std::uint32_t Seconds;

    /// @brief タイムスタンプ（マイクロ秒）
    std::uint32_t Microseconds;

    /// @brief 収録されたデータの長さ
    std::uint32_t IncludedLength;

    /// @brief オリジナルの長さ
    std::uint32_t OriginalLength;
};
} // namespace PacketEntity
#endif
