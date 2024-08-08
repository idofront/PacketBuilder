#ifndef JSON_CONVERTER__UDP_HPP__
#define JSON_CONVERTER__UDP_HPP__

#include <StackableEntity.hpp>
#include <Udp.hpp>

namespace JsonConverter
{
class UdpEntity;
using UdpEntityPtr = std::shared_ptr<UdpEntity>;
class UdpEntity : public StackableEntity
{
  public:
    uint16_t SourcePort;
    uint16_t DestinationPort;
    uint16_t Length;
    uint16_t Checksum;
};
} // namespace JsonConverter
#endif
