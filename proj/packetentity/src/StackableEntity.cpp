#include <StackableEntity.hpp>
#include <Utility.hpp>

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
        auto typeName = typeid(*this).name();
        auto demangledName = Utility::Demangle(typeName);
        json["EntityType"] = demangledName;
    }

    return json;
}

StackableEntityPtr FromJson(nlohmann::json json)
{
    throw std::runtime_error("Not implemented");
}
} // namespace PacketEntity
