#ifndef JSON_CONVERTER__IPV4_ENTITY_HPP__
#define JSON_CONVERTER__IPV4_ENTITY_HPP__

#include <Ipv4.hpp>
#include <StackableEntity.hpp>

namespace JsonConverter
{
class Ipv4Entity;
using Ipv4EntityPtr = std::shared_ptr<Ipv4Entity>;
class Ipv4Entity : public StackableEntity
{
  public:
    uint8_t Version;
    uint8_t IHL;
    uint8_t DSCP;
    uint16_t ECN;
    uint16_t TotalLength;
    uint16_t Identification;
    uint16_t Flags;
    uint16_t FragmentOffset;
    uint8_t TTL;
    uint8_t Protocol;
    uint16_t HeaderChecksum;
    uint32_t SourceAddress;
    uint32_t DestinationAddress;
};
} // namespace JsonConverter

#endif
