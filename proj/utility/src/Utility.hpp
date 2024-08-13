#ifndef UTILITY__UTILITY_HPP__
#define UTILITY__UTILITY_HPP__

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <cstdint>
#include <cxxabi.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

namespace Utility
{
using DataArray = uint8_t[];
using DataArrayPtr = std::shared_ptr<DataArray>;

/// @brief
/// @param demangle
/// @return
std::string Demangle(const std::string &demangle);

/// @brief
/// @param data
/// @param length
/// @return
std::string HexDump(DataArrayPtr data, std::size_t length);
} // namespace Utility

#endif
