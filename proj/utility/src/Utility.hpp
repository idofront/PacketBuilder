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

/// @brief デマングルした文字列を返す．
/// @param demangle デマングルする文字列
/// @return デマングルした文字列
std::string Demangle(const std::string &demangle);

/// @brief データ配列を 16 進数文字列に変換する．
/// @param data データ配列
/// @param length データ長
/// @return 16 進数文字列
std::string HexDump(DataArrayPtr data, std::size_t length);

/// @brief Ethernet アドレスを文字列に変換する．
/// @param addr 変換後の文字列
/// @param ether_addr 変換前の Ethernet アドレス
void EthernetAddressToString(std::string &addr, const uint8_t *const ether_addr);

/// @brief 文字列を Ethernet アドレスに変換する．
/// @param addr 変換前の文字列
/// @param ether_addr 変換後の Ethernet アドレス
void EthernetAddressFromString(const std::string &addr, uint8_t *const ether_addr);

} // namespace Utility

#endif
