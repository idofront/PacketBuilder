#ifndef PACKET_ENTITY__ENTITY_SERVICE_HPP
#define PACKET_ENTITY__ENTITY_SERVICE_HPP

#include <AbsoluteEntity.hpp>
#include <BinaryEntity.hpp>
#include <EthernetEntity.hpp>
#include <Ipv4Entity.hpp>
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
    class ParsePcapHelper
    {
      public:
        static StackableEntityPtr Parse(const uint8_t *packet, std::size_t length);
        static StackableEntityPtr ParseEthernet(const uint8_t *packet, std::size_t length);
        static StackableEntityPtr ParseIpv4(const uint8_t *packet, std::size_t length);
        static StackableEntityPtr ParseUdp(const uint8_t *packet, std::size_t length);
        static StackableEntityPtr ParseBinary(const uint8_t *packet, std::size_t length);

      private:
        using NextEthernetFactoryMap =
            std::map<uint16_t, std::function<StackableEntityPtr(const uint8_t *, std::size_t)>>;
        static NextEthernetFactoryMap NextEthernetFactory;
    };
};
} // namespace PacketEntity

#endif
