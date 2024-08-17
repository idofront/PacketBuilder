#ifndef PACKET_BUILDER__ETHERNET_HEADER_HPP__
#define PACKET_BUILDER__ETHERNET_HEADER_HPP__

#include <EthernetEntity.hpp>
#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <netinet/ether.h>

namespace Packet
{
class Ethernet;
using EthernetPtr = std::shared_ptr<Ethernet>;
/// @brief Ethernetフレームのヘッダを表現する．
class Ethernet : public Stackable
{
  public:
    Ethernet();
    Ethernet(PacketEntity::EthernetEntityPtr entity);
    virtual ~Ethernet();

  public:
    /// @brief 宛先MACアドレス
    Utility::NotifyProperty<uint8_t *> DestinationMac; // TODO メモリ直利用を避ける

    /// @brief 送信元MACアドレス
    Utility::NotifyProperty<uint8_t *> SourceMac; // TODO メモリ直利用を避ける

    /// @brief イーサネットタイプ
    Utility::NotifyProperty<uint16_t> EthernetType;

  private:
    static const std::size_t HeaderSize = sizeof(struct ether_header);
    struct ether_header *Header() const;
    PacketEntity::EthernetEntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet
#endif
