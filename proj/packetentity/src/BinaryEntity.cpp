#include <BinaryEntity.hpp>

namespace PacketEntity
{
BinaryEntity::BinaryEntity(std::size_t length) : StackableEntity(), Data(std::make_shared<DataArray>(length))
{
}

BinaryEntity::~BinaryEntity()
{
}

nlohmann::json BinaryEntity::ToJson()
{
    auto json = StackableEntity::ToJson();
    json["Data"] = *(this->Data);
    return json;
}

} // namespace PacketEntity
