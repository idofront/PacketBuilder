#ifndef JSON_CONVERTER__CONVERTER_HPP__
#define JSON_CONVERTER__CONVERTER_HPP__

#include <JCEthernetHeaderEntity.hpp>
#include <JCIpv4Entity.hpp>
#include <JCUdpEntity.hpp>
#include <Udp.hpp>
#include <nlohmann/json.hpp>

namespace JsonConverter
{
void to_json(nlohmann::json &j, const EthernetHeaderEntity &entity);
void from_json(const nlohmann::json &j, EthernetHeaderEntity &entity);
void to_entity(EthernetHeaderEntity &entity, const Packet::Ethernet &header);
void from_entity(const EthernetHeaderEntity &entity, Packet::Ethernet &header);
void EthernetAddressToString(std::string &addr, const uint8_t *const ether_addr);
void EthernetAddressFromString(const std::string &addr, uint8_t *const ether_addr);

void to_json(nlohmann::json &j, const Ipv4Entity &entity);
void from_json(const nlohmann::json &j, Ipv4Entity &entity);
void to_entity(Ipv4Entity &entity, const Packet::Ipv4 &header);
void from_entity(const Ipv4Entity &entity, Packet::Ipv4 &header);

void to_json(nlohmann::json &j, const UdpEntity &entity);
void from_json(const nlohmann::json &j, UdpEntity &entity);
void to_entity(UdpEntity &entity, const Packet::Udp &header);
void from_entity(const UdpEntity &entity, Packet::Udp &header);

void to_json(nlohmann::json &j, const StackableEntity &entity);
void from_json(const nlohmann::json &j, StackableEntity &entity);
void to_entity(StackableEntity &entity, const Packet::Stackable &header);
void from_entity(const StackableEntity &entity, Packet::Stackable &header);

StackableEntityPtr CreateStackableEntity(const std::string &type);
StackableEntityPtr CreateStackableEntity(const std::string &type, const nlohmann::json &j);
} // namespace JsonConverter

#endif