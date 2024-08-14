#ifndef PACKET_ENTITY__ETHERNET_ENTITY_HPP
#define PACKET_ENTITY__ETHERNET_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class EthernetEntity;
using EthernetEntityPtr = std::shared_ptr<EthernetEntity>;
class EthernetEntity : public StackableEntity
{
  public:
    EthernetEntity();
    virtual ~EthernetEntity();
    virtual nlohmann::json ToJson() override;

  public:
    std::string DestinationMac;
    std::string SourceMac;
    uint16_t Type;
};
} // namespace PacketEntity
#endif
