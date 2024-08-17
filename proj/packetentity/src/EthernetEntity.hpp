#ifndef PACKET_ENTITY__ETHERNET_ENTITY_HPP
#define PACKET_ENTITY__ETHERNET_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class EthernetEntity;
using EthernetEntityPtr = std::shared_ptr<EthernetEntity>;
/// @brief Ethernet ヘッダを表現する．
class EthernetEntity : public StackableEntity
{
  public:
    EthernetEntity();
    virtual ~EthernetEntity();
    virtual nlohmann::json ToJson() override;
    static StackableEntityPtr FromJson(nlohmann::json json);

  public:
    /// @brief 宛先 MAC アドレス
    std::string DestinationMac;

    /// @brief 送信元 MAC アドレス
    std::string SourceMac;

    /// @brief タイプ
    uint16_t Type;
};
} // namespace PacketEntity
#endif
