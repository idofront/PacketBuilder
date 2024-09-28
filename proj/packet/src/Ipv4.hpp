#ifndef PACKET_BUILDER__IPV4_HPP__
#define PACKET_BUILDER__IPV4_HPP__

#include <Ipv4Entity.hpp>
#include <ObservableProperty.hpp>
#include <Stackable.hpp>
#include <netinet/ip.h>

namespace Packet
{
class Ipv4;
using Ipv4Ptr = std::shared_ptr<Ipv4>;
/// @brief IPv4ヘッダを表現する．
class Ipv4 : public Stackable
{
  public:
    Ipv4();
    Ipv4(PacketEntity::Ipv4EntityPtr entity);
    ~Ipv4();

  public:
    /// @brief バージョン
    Utility::ObservableProperty<int> Version;

    /// @brief ヘッダ長
    Utility::ObservableProperty<int> Ihl;

    /// @brief サービスタイプ
    Utility::ObservableProperty<uint8_t> Tos;

    /// @brief トータル長
    Utility::ObservableProperty<uint16_t> TotalLength;

    /// @brief 識別子
    Utility::ObservableProperty<uint16_t> Id;

    /// @brief フラグ
    Utility::ObservableProperty<uint16_t> Flags;

    /// @brief フラグメントオフセット
    Utility::ObservableProperty<uint16_t> FragmentOffset;

    /// @brief TTL
    Utility::ObservableProperty<uint8_t> Ttl;

    /// @brief プロトコル
    Utility::ObservableProperty<uint8_t> Protocol;

    /// @brief チェックサム
    Utility::ObservableProperty<uint16_t> Checksum;

    /// @brief 送信元アドレス
    Utility::ObservableProperty<sockaddr_in> SourceAddress;

    /// @brief 宛先アドレス
    Utility::ObservableProperty<sockaddr_in> DestinationAddress;

  protected:
    virtual void OnStacked(StackablePtr oldStackable, StackablePtr newStackable) override;

  private:
    using iphdr_t = struct iphdr;
    static const std::size_t HeaderSize = sizeof(iphdr_t);
    iphdr_t *Ipv4Header() const;
    static uint16_t CalculateChecksum(const Ipv4 *ipv4);
    PacketEntity::Ipv4EntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet

#endif
