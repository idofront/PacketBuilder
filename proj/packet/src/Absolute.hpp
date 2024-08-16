#ifndef PACKET__ABSOLUTE_HPP
#define PACKET__ABSOLUTE_HPP

#include <AbsoluteEntity.hpp>
#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <chrono>

namespace Packet
{
class Absolute;
using AbsolutePtr = std::shared_ptr<Absolute>;
class Absolute : public Stackable
{
  public:
    Absolute();
    Absolute(PacketEntity::AbsoluteEntityPtr entity);
    virtual ~Absolute();

  public:
    Utility::NotifyProperty<std::chrono::nanoseconds> TimestampNs;

  private:
    PacketEntity::AbsoluteEntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet

#endif
