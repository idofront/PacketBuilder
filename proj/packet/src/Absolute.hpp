#ifndef PACKET__ABSOLUTE_HPP
#define PACKET__ABSOLUTE_HPP

#include <AbsoluteEntity.hpp>
#include <NotifyProperty.hpp>
#include <Stackable.hpp>
#include <chrono>

namespace Packet
{
/// @brief パケット到着時刻を絶対時刻で表現する
class Absolute;
using AbsolutePtr = std::shared_ptr<Absolute>;
class Absolute : public Stackable
{
  public:
    Absolute();
    Absolute(PacketEntity::AbsoluteEntityPtr entity);
    virtual ~Absolute();

  public:
    /// @brief パケット到着時刻
    Utility::NotifyProperty<std::chrono::nanoseconds> TimestampNs;

  private:
    PacketEntity::AbsoluteEntityPtr Entity();
    void RegisterCallbacks();
};
} // namespace Packet

#endif
