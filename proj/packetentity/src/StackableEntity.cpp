#include <StackableEntity.hpp>
#include <Utility.hpp>
#include <nameof/nameof.h>

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
    auto typeName = typeid(*this).name();
    auto demangledName = Utility::Demangle(typeName);

    auto json = nlohmann::json();

    if (this->Stack.Value())
    {
        json[nameof(this->Stack)] = this->Stack.Value()->ToJson();
    }

    json["EntityType"] = this->EntityType();

    return json;
}

std::string StackableEntity::EntityType()
{
    auto typeName = typeid(*this).name();
    auto demangledName = Utility::Demangle(typeName);
    return demangledName;
}
} // namespace PacketEntity
