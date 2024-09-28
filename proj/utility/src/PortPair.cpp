#include <PortPair.hpp>

namespace Utility
{
PortPair::PortPair(uint16_t sourcePort, uint16_t destinationPort) : m_Ports({sourcePort, destinationPort})
{
}

bool PortPair::operator==(const PortPair &rhs) const
{
    return m_Ports.Ports == rhs.m_Ports.Ports;
}

bool PortPair::operator!=(const PortPair &rhs) const
{
    return m_Ports.Ports != rhs.m_Ports.Ports;
}

bool PortPair::operator<(const PortPair &rhs) const
{
    return m_Ports.Ports < rhs.m_Ports.Ports;
}

bool PortPair::operator>(const PortPair &rhs) const
{
    return m_Ports.Ports > rhs.m_Ports.Ports;
}

bool PortPair::operator<=(const PortPair &rhs) const
{
    return m_Ports.Ports <= rhs.m_Ports.Ports;
}

bool PortPair::operator>=(const PortPair &rhs) const
{
    return m_Ports.Ports >= rhs.m_Ports.Ports;
}

} // namespace Utility
