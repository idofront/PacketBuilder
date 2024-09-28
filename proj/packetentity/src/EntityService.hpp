#ifndef PACKET_ENTITY__ENTITY_SERVICE_HPP
#define PACKET_ENTITY__ENTITY_SERVICE_HPP

#include <AbsoluteEntity.hpp>
#include <BinaryEntity.hpp>
#include <EthernetEntity.hpp>
#include <Ipv4Entity.hpp>
#include <PortPair.hpp>
#include <StackableEntity.hpp>
#include <UdpEntity.hpp>
#include <Utility.hpp>
#include <boost/format.hpp>
#include <map>
#include <pcap.h>

namespace PacketEntity
{
/// @brief エンティティに対する操作を提供する．
class EntityService
{
  public:
    /// @brief JSON からエンティティを生成する．
    /// @param json JSON
    /// @return エンティティ
    static StackableEntityPtr ParseEntity(nlohmann::json json);

    /// @brief JSON (array) からエンティティの配列を生成する．
    /// @param json JSON
    /// @return エンティティの配列
    static std::vector<StackableEntityPtr> ParseEntities(const std::filesystem::path &filepath);

    /// @brief PCAP パケットからエンティティを生成する．
    /// @param header パケットヘッダ
    /// @param packet パケットデータ
    /// @return エンティティ
    static StackableEntityPtr ParsePcap(const struct pcap_pkthdr *header, const uint8_t *packet);

    /// @brief PCAP ファイルからエンティティの配列を生成する．
    /// @param filepath ファイルパス
    /// @return エンティティの配列
    static std::vector<StackableEntityPtr> ParsePcap(const std::filesystem::path &filepath);

  private:
    /// @brief エンティティのファクトリ関数群
    class ParsePcapHelper
    {
      public:
        /// @brief パケットデータからエンティティを生成する．
        /// @param packet パケットデータ
        /// @param length パケット長
        /// @return エンティティ
        static StackableEntityPtr Parse(const uint8_t *packet, std::size_t length);

        /// @brief Ethernet ヘッダを解析する．
        /// @param packet パケットデータ
        /// @param length パケット長
        /// @return エンティティ
        static StackableEntityPtr ParseEthernet(const uint8_t *packet, std::size_t length);

        /// @brief IPv4 ヘッダを解析する．
        /// @param packet パケットデータ
        /// @param length パケット長
        /// @return エンティティ
        static StackableEntityPtr ParseIpv4(const uint8_t *packet, std::size_t length);

        /// @brief UDP ヘッダを解析する．
        /// @param packet パケットデータ
        /// @param length パケット長
        /// @return エンティティ
        static StackableEntityPtr ParseUdp(const uint8_t *packet, std::size_t length);

        /// @brief TCP ヘッダを解析する．
        /// @param packet パケットデータ
        /// @param length パケット長
        /// @return エンティティ
        static StackableEntityPtr ParseTcp(const uint8_t *packet, std::size_t length);

        /// @brief バイナリデータを解析する．
        /// @param packet パケットデータ
        /// @param length パケット長
        /// @return エンティティ
        static StackableEntityPtr ParseBinary(const uint8_t *packet, std::size_t length);

      private:
        using FactoryFunction = std::function<StackableEntityPtr(const uint8_t *, std::size_t)>;
        static StackableEntityPtr CreateStackableEntity(FactoryFunction &factory, const uint8_t *packet,
                                                        std::size_t length) noexcept;

        using NextEthernetFactoryMap = std::map<uint16_t, FactoryFunction>;
        static NextEthernetFactoryMap NextEthernetFactory;

        using NextIpv4FactoryMap = std::map<uint8_t, FactoryFunction>;
        static NextIpv4FactoryMap NextIpFactory;

        using NextTransportFactoryMap = std::map<Utility::PortPair, FactoryFunction>;
        static NextTransportFactoryMap NextTransportFactory;
    };
};
} // namespace PacketEntity

#endif
