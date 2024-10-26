#ifndef PLUGIN_CONTRACT__BINARY_HPP__
#define PLUGIN_CONTRACT__BINARY_HPP__

#include <ObservableProperty.hpp>
#include <Packet/Stackable.hpp>
#include <Utility.hpp>
#include <vector>

namespace PluginContract
{
namespace Packet
{
class Binary;
using BinaryPtr = std::shared_ptr<Binary>;

/// @brief バイナリデータを表現する．
class Binary : public PluginContract::Packet::Stackable
{
  public:
    Binary(std::size_t length);
    Binary(std::vector<uint8_t> data);
    Binary(uint8_t *data, std::size_t length);
    virtual ~Binary() override;
};
} // namespace Packet
} // namespace PluginContract

#endif
