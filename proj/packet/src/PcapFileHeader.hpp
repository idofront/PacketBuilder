#ifndef PACKET_BUILDER__PCAP_FILE_HEADER_HPP__
#define PACKET_BUILDER__PCAP_FILE_HEADER_HPP__

#include <NotifyProperty.hpp>
#include <PcapFileHeaderEntity.hpp>
#include <Stackable.hpp>
#include <pcap.h>

namespace Packet
{
class PcapFileHeader;
using PcapFileHeaderPtr = std::shared_ptr<PcapFileHeader>;
/// @brief Pcapファイルヘッダを表現する．
class PcapFileHeader : public Stackable
{
  public:
    PcapFileHeader();

    /// @brief マジックナンバーを設定/取得する．
    Utility::NotifyProperty<uint32_t> MagicNumber;

    /// @brief メジャーバージョンを設定/取得する．
    Utility::NotifyProperty<uint16_t> VersionMajor;

    /// @brief マイナーバージョンを設定/取得する．
    Utility::NotifyProperty<uint16_t> VersionMinor;

    /// @brief タイムゾーンを設定/取得する．
    Utility::NotifyProperty<uint32_t> ThisZone;

    /// @brief タイムスタンプの精度を設定/取得する．
    Utility::NotifyProperty<uint32_t> SigFigs;

    /// @brief キャプチャファイルの最大長を設定/取得する．
    Utility::NotifyProperty<uint32_t> SnapLen;

    /// @brief ネットワークを設定/取得する．
    Utility::NotifyProperty<uint32_t> LinkType;

  private:
    static const std::size_t HeaderSize = sizeof(struct ::pcap_file_header);
    struct pcap_file_header *Header() const;
    PacketEntity::PcapFileHeaderEntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet

#endif
