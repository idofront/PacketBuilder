#ifndef JSON_CONVERTER__ETHERNET_HEADER_ENTITY_HPP__
#define JSON_CONVERTER__ETHERNET_HEADER_ENTITY_HPP__

#include <EthernetHeader.hpp>
#include <StackableEntity.hpp>
#include <netinet/ether.h>
#include <nlohmann/json.hpp>

namespace JsonConverter
{
struct EthernetHeaderEntity : public StackableEntity
{
    std::string DestinationMac;
    std::string SourceMac;
    uint16_t EthernetType;
};

} // namespace JsonConverter
#endif
