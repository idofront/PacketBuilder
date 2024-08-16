#ifndef PACKET__BINARY_HPP
#define PACKET__BINARY_HPP

#include <Stackable.hpp>

namespace Packet
{
class Binary;
using BinaryPtr = std::shared_ptr<Binary>;
class Binary : public Stackable
{
  public:
    Binary(std::size_t length);
    virtual ~Binary();
};
} // namespace Packet
#endif
