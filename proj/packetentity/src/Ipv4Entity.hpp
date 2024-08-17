#ifndef PACKET_ENTITY__IPV4_ENTITY_HPP
#define PACKET_ENTITY__IPV4_ENTITY_HPP

#include <StackableEntity.hpp>
namespace PacketEntity
{
class Ipv4Entity;
using Ipv4EntityPtr = std::shared_ptr<Ipv4Entity>;
/// @brief IPv4 ヘッダを表現する．
class Ipv4Entity : public StackableEntity
{
  public:
    Ipv4Entity();
    virtual ~Ipv4Entity();
    virtual nlohmann::json ToJson() override;
    static StackableEntityPtr FromJson(nlohmann::json json);

  public:
    /// @brief バージョン
    uint8_t Version;

    /// @brief ヘッダ長
    uint8_t IHL;

    /// @brief サービスタイプ
    uint8_t DSCP;

    /// @brief ECN
    uint16_t ECN;

    /// @brief パケット全体の長さ
    uint16_t TotalLength;

    /// @brief 識別子
    uint16_t Identification;

    /// @brief フラグ
    uint16_t Flags;

    /// @brief フラグメントオフセット
    uint16_t FragmentOffset;

    /// @brief TTL
    uint8_t TTL;

    /// @brief プロトコル
    uint8_t Protocol;

    /// @brief ヘッダチェックサム
    uint16_t HeaderChecksum;

    /// @brief 送信元 IP アドレス
    uint32_t SourceAddress;

    /// @brief 宛先 IP アドレス
    uint32_t DestinationAddress;
};
} // namespace PacketEntity

#endif
