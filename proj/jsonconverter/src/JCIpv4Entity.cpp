#include <JCIpv4Entity.hpp>

#include <boost/format.hpp>

namespace JsonConverter
{
std::string Ipv4Entity::ToString()
{
    const auto &classTypeInfo = typeid(*this);
    auto demangledName = PacketBuilder::Utility::Demangle(classTypeInfo.name());
    auto fmt =
        boost::format("\n%1%\n\tStackType: %2%\n\tVersion: %3%\n\tIHL: %4%\n\tDSCP: %5%\n\tECN: %6%\n\tTotalLength: "
                      "%7%\n\tIdentification: %8%\n\tFlags: %9%\n\tFragmentOffset: %10%\n\tTTL: %11%\n\tProtocol: "
                      "%12%\n\tHeaderChecksum: %13%\n\tSourceIp: %14%\n\tDestinationIp: %15%");
    auto str = fmt % demangledName % this->StackType % this->Version % this->IHL % this->DSCP % this->ECN %
               this->TotalLength % this->Identification % this->Flags % this->FragmentOffset % this->TTL %
               this->Protocol % this->HeaderChecksum % this->SourceAddress % this->DestinationAddress;
    return str.str();
}
} // namespace JsonConverter
