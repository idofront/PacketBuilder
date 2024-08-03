#ifndef PACKET_BUILDER__UTILITY__UTILITY_HPP__
#define PACKET_BUILDER__UTILITY__UTILITY_HPP__

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <cstdint>
#include <cxxabi.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace PacketBuilder::Utility
{
using DataArray = uint8_t[];
using DataArrayPtr = std::shared_ptr<DataArray>;

/// @brief
/// @param data
/// @param length
/// @return
std::string HexDump(DataArrayPtr data, std::size_t length);

/// @brief
/// @param demangle
/// @return
std::string Demangle(const std::string &demangle);

} // namespace PacketBuilder::Utility

#endif
