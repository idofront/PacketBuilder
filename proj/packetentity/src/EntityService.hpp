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
class EntityService
{
  public:
    static StackableEntityPtr ParseEntity(nlohmann::json json);
    static std::vector<StackableEntityPtr> ParseEntities(nlohmann::json json);
};
} // namespace PacketEntity

#endif
