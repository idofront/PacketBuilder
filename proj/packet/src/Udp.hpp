#ifndef PACKET_BUILDER__UDP_HPP__
#define PACKET_BUILDER__UDP_HPP__

#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <UdpEntity.hpp>
#include <netinet/udp.h>

namespace Packet
{
class Udp;
using UdpPtr = std::shared_ptr<Udp>;
/// @brief UDP ヘッダを表現する．
class Udp : public Stackable
{
  public:
    Udp();
    Udp(PacketEntity::UdpEntityPtr entity);
    virtual ~Udp();

  public:
    /// @brief ソースポートを設定/取得する．
    Utility::NotifyProperty<uint16_t> SourcePort;

    /// @brief デスティネーションポートを設定/取得する．
    Utility::NotifyProperty<uint16_t> DestinationPort;

    /// @brief UDP の長さを取得する．
    /// @details UDP ヘッダの長さを含む．
    Utility::NotifyProperty<uint16_t> UdpLength;

    /// @brief チェックサムを設定/取得する．
    Utility::NotifyProperty<uint16_t> UdpChecksum;

  protected:
    virtual void OnStacked(StackablePtr oldStackable, StackablePtr newStackable) override;

  private:
    static const std::size_t HeaderSize = sizeof(struct udphdr);
    struct udphdr *UdpHeader();
    void RegisterCallbacks();
    PacketEntity::UdpEntityPtr Entity();
};
} // namespace Packet

#endif
