#ifndef PACKET_ENTITY__ETHERNET_DOT1Q_ENTITY_HPP
#define PACKET_ENTITY__ETHERNET_DOT1Q_ENTITY_HPP

#include <EthernetEntity.hpp>

namespace PacketEntity
{
class EthernetDot1qEntity;
using EthernetDot1qEntityPtr = std::shared_ptr<EthernetDot1qEntity>;
/// @brief Ethernet ヘッダを表現する．
class EthernetDot1qEntity : public EthernetEntity
{
  public:
    EthernetDot1qEntity() = default;
    virtual ~EthernetDot1qEntity();
    virtual nlohmann::json ToJson() override;
    static StackableEntityPtr FromJson(nlohmann::json json);

  public:
    class Dot1qHeader
    {
      public:
        Dot1qHeader() = default;

        /// @brief Tag Protocol Identifier
        uint16_t Tpid;

        /// @brief Priority Code Point
        union TagControl_u {
            struct
            {
                uint16_t UserPriority : 3;
                uint16_t CanonicalFormat : 1;
                uint16_t VlanIdentifier : 12;
            };
            uint16_t Value;
        } TagControl;
    };

    /// @brief 802.1Q ヘッダ
    Dot1qHeader VlanHeader;
};
} // namespace PacketEntity

#endif
