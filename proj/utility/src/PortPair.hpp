#ifndef UTLITY__PORT_PAIR_HPP
#define UTLITY__PORT_PAIR_HPP

#include <cstdint>

namespace Utility
{
class PortPair
{
  public:
    PortPair(uint16_t sourcePort, uint16_t destinationPort);
    bool operator==(const PortPair &rhs) const;
    bool operator!=(const PortPair &rhs) const;
    bool operator<(const PortPair &rhs) const;
    bool operator>(const PortPair &rhs) const;
    bool operator<=(const PortPair &rhs) const;
    bool operator>=(const PortPair &rhs) const;

  private:
    union {
        struct
        {
            uint16_t SourcePort;
            uint16_t DestinationPort;
        };
        uint32_t Ports;
    } m_Ports;
};
} // namespace Utility

#endif
