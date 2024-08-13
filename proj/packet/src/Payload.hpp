#ifndef PACKET_BUILDER__PAYLOAD_HPP__
#define PACKET_BUILDER__PAYLOAD_HPP__

#include <Stackable.hpp>
#include <Utility/Utility.hpp>

namespace Packet
{
class Payload : public Stackable
{
  public:
    Payload(std::size_t length);
    virtual ~Payload() = default;

  private:
};
} // namespace PacketBuilder

#endif
