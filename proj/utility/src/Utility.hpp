#ifndef UTILITY__UTILITY_HPP__
#define UTILITY__UTILITY_HPP__

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <boost/format.hpp>
#include <cstdint>
#include <cxxabi.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <typeindex>

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

/// @brief JSON オブジェクトから指定したキーの値を取得する．
template <class T>
T ParseJsonObjectHelper(nlohmann::json &json, const std::string &key, T default_value,
                        std::function<bool(T)> validator = nullptr)
{
    if (!json.is_object())
    {
        throw std::runtime_error("The json object is not an object.");
    }

    if (json.find(key) == json.end())
    {
        SPDLOG_WARN("The key is not found in json object: {}", key);
        return default_value;
    }

    try
    {
        auto value = json[key].get<T>();
        if (validator != nullptr && !validator(value))
        {
            auto fmt = boost::format("Validation failed.");
            throw std::runtime_error(fmt.str());
        }

        return value;
    }
    catch (const nlohmann::json::type_error &e)
    {
        // key を取得できたが型が異なる場合
        SPDLOG_WARN("Failed to parse json object: {}", e.what());
        auto fmt = boost::format("The key '%1%' is found but invalid type. Return default value.");
        auto msg = fmt % key;
        SPDLOG_WARN(msg.str());

        return default_value;
    }
}
} // namespace Utility

#endif
