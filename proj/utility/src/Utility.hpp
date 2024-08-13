#ifndef UTILITY__UTILITY_HPP__
#define UTILITY__UTILITY_HPP__

#include <cstdint>
#include <cxxabi.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

namespace Utility
{
/// @brief
/// @param demangle
/// @return
std::string Demangle(const std::string &demangle);
} // namespace Utility

#endif
