#ifndef PACKET_ENTITY__BINARY_ENTITY_HPP
#define PACKET_ENTITY__BINARY_ENTITY_HPP

#include <StackableEntity.hpp>

namespace PacketEntity
{
using DataArray = std::vector<uint8_t>;
using DataArrayPtr = std::shared_ptr<DataArray>;

class BinaryEntity : public StackableEntity
{
  public:
    BinaryEntity(std::size_t length);
    virtual ~BinaryEntity();
    virtual nlohmann::json ToJson() override;

  public:
    DataArrayPtr Data;
};
} // namespace PacketEntity

#endif
