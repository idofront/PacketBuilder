#include <StackableEntity.hpp>

namespace PacketEntity
{
StackableEntity::StackableEntity() : Stack(nullptr)
{
}

StackableEntity::~StackableEntity()
{
}

std::string StackableEntity::ToString()
{
    auto json = this->ToJson();
    return json.dump();
}

nlohmann::json StackableEntity::ToJson()
{
    auto json = nlohmann::json();

    if (this->Stack.Value())
    {
        json["Stack"] = this->Stack.Value()->ToJson();
    }

    return json;
}
} // namespace PacketEntity
