#ifndef PACKET_ENTITY__PCAP_FILE_HEADER_ENTITY_HPP
#define PACKET_ENTITY__PCAP_FILE_HEADER_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class PcapFileHeaderEntity;
using PcapFileHeaderEntityPtr = std::shared_ptr<PcapFileHeaderEntity>;
/// @brief Pcap ファイルヘッダを表現する．
class PcapFileHeaderEntity : public StackableEntity
{
  public:
    PcapFileHeaderEntity();
    virtual ~PcapFileHeaderEntity();
    virtual nlohmann::json ToJson() override;

  public:
    /// @brief マジックナンバー
    std::uint32_t MagicNumber;

    /// @brief メジャーバージョン
    std::uint16_t MajorVersion;

    /// @brief マイナーバージョン
    std::uint16_t MinorVersion;

    /// @brief タイムゾーン
    std::uint32_t ThisZone;

    /// @brief タイムスタンプの精度
    std::uint32_t SigFigs;

    /// @brief キャプチャファイルの最大長
    std::uint32_t SnapLen;

    /// @brief ネットワーク
    std::uint32_t Network;
};
} // namespace PacketEntity
#endif
