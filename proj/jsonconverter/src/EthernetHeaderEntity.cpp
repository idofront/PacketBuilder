#include <EthernetHeaderEntity.hpp>
#include <Utility.hpp>
#include <boost/format.hpp>
#include <typeinfo>

namespace JsonConverter
{
std::string EthernetHeaderEntity::ToString()
{
    const auto &classTypeInfo = typeid(*this);
    auto demangledName = PacketBuilder::Utility::Demangle(classTypeInfo.name());
    auto fmt = boost::format("\n%1%\n\tStackType: %2%\n\tDestinationMac: %3%\n\tSourceMac: %4%\n\tEthernetType: %5%");
    auto str = fmt % demangledName % this->StackType % this->DestinationMac % this->SourceMac % this->EthernetType;
    return str.str();
}
} // namespace JsonConverter
