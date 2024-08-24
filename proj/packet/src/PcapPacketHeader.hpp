#ifndef PACKET_BUILDER__PCAP_PACKET_HEADER_HPP__
#define PACKET_BUILDER__PCAP_PACKET_HEADER_HPP__

#include <ObservableProperty.hpp>
#include <PcapPacketHeaderEntity.hpp>
#include <Stackable.hpp>
#include <chrono>
#include <pcap.h>

namespace Packet
{
/// @brief Pcapパケットヘッダを表現する．
/// @details 通常，struct pcap_pkthdr を利用するが，データ長 sizeof が正しく取得できないため，独自に定義する．
struct RecordHeader
{
    uint32_t timestampSeconds;
    uint32_t timestampMicroseconds;
    uint32_t includedLength;
    uint32_t originalLength;
};

class PcapPacketHeader;
using PcapPacketHeaderPtr = std::shared_ptr<PcapPacketHeader>;
/// @brief Pcapパケットヘッダを表現する．
class PcapPacketHeader : public Stackable
{
  public:
    PcapPacketHeader();

    /// @brief 時刻 (秒) を取得する．
    Utility::ObservableProperty<std::chrono::seconds> TimestampSeconds;

    /// @brief 時刻 (マイクロ秒) を取得する．
    Utility::ObservableProperty<std::chrono::microseconds> TimestampMicroseconds;

    /// @brief 収録データ長を取得する．
    Utility::ObservableProperty<uint32_t> IncludedLength;

    /// @brief オリジナルデータ長を取得する．
    Utility::ObservableProperty<uint32_t> OriginalLength;

  protected:
    virtual void OnStacked(StackablePtr oldStackable, StackablePtr newStackable) override;

  private:
    static const std::size_t HeaderSize = sizeof(struct RecordHeader);
    struct RecordHeader *Header() const;

    PacketEntity::PcapPacketHeaderEntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet

#endif
