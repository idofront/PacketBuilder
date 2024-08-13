#ifndef PACKET_ENTITY__UDP_ENTITY_HPP
#define PACKET_ENTITY__UDP_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
class UdpEntity;
using UdpEntityPtr = std::shared_ptr<UdpEntity>;
class UdpEntity : public StackableEntity
{
  public:
    UdpEntity();
    virtual ~UdpEntity();
    virtual nlohmann::json ToJson() override;

  public:
    uint16_t SourcePort;
    uint16_t DestinationPort;
    uint16_t Length;
    uint16_t Checksum;
};
} // namespace PacketEntity

#endif
