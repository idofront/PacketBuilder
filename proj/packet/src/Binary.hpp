#ifndef PACKET__BINARY_HPP
#define PACKET__BINARY_HPP

#include <BinaryEntity.hpp>
#include <Stackable.hpp>

namespace Packet
{
class Binary;
using BinaryPtr = std::shared_ptr<Binary>;
class Binary : public Stackable
{
  public:
    Binary(std::size_t length);
    Binary(PacketEntity::BinaryEntityPtr entity);
    virtual ~Binary();
};
} // namespace Packet
#endif
