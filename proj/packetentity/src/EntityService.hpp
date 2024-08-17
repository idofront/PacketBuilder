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
    static std::vector<StackableEntityPtr> ParseEntities(nlohmann::json json);
};
} // namespace PacketEntity

#endif
