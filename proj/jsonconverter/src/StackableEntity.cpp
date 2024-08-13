#include <StackableEntity.hpp>
#include <boost/format.hpp>

namespace JsonConverter
{
StackableEntity::StackableEntity() : StackType("")
{
}

StackableEntity::~StackableEntity()
{
}

std::string StackableEntity::ToString()
{
    auto fmt = boost::format(R"({"StackType": "%1%"})");
    auto str = fmt % this->StackType;
    return str.str();
}
} // namespace JsonConverter
