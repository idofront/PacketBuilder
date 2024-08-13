#ifndef JSON_CONVERTER__ETHERNET_HEADER_ENTITY_HPP__
#define JSON_CONVERTER__ETHERNET_HEADER_ENTITY_HPP__

#include <EthernetHeader.hpp>
#include <JCStackableEntity.hpp>
#include <netinet/ether.h>
#include <nlohmann/json.hpp>

namespace JsonConverter
{
class EthernetHeaderEntity : public StackableEntity
{
  public:
    std::string DestinationMac;
    std::string SourceMac;
    uint16_t EthernetType;
    virtual std::string ToString() override;
};

} // namespace JsonConverter
#endif
