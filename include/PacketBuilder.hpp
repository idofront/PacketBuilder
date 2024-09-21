#ifndef PACKET_BUILDER__PACKET_BUILDER_HPP__
#define PACKET_BUILDER__PACKET_BUILDER_HPP__

#include <Absolute.hpp>
#include <ObservableProperty.hpp>
#include <Options.hpp>
#include <PcapPacketHeader.hpp>
#include <algorithm>
#include <boost/format.hpp>
#include <cmdline/cmdline.h>
#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>

/// @brief ログレベルを文字列から変換する．
/// @param str ログレベルの文字列
/// @return ログレベル
inline spdlog::level::level_enum LogLevelFromString(std::string str)
{
    std::map<std::string, spdlog::level::level_enum> logLevelMap = {
        {"trace", spdlog::level::trace}, {"debug", spdlog::level::debug}, {"info", spdlog::level::info},
        {"warn", spdlog::level::warn},   {"error", spdlog::level::err},   {"critical", spdlog::level::critical},
        {"off", spdlog::level::off}};
    auto it = logLevelMap.find(str);
    if (it == logLevelMap.end())
    {
        throw std::runtime_error("Invalid log level");
    }
    return it->second;
}

/// @brief コマンドライン引数のパーサを取得する．
/// @return コマンドライン引数のパーサ
inline cmdline::parser GetArgumentParser()
{
    cmdline::parser parser;

    // Add options
    // Output file name.
    parser.add<std::string>("output", 'o', "Output file name", false, "");

    parser.add<std::string>("input", 'i', "Input file name", false, "");

    // Log level.
    parser.add<std::string>("log", 'l', "Log level", false, "info",
                            cmdline::oneof<std::string>("trace", "debug", "info", "warn", "error", "critical"));

    return parser;
}

/// @brief コマンドライン引数を解析する．
/// @param options 解析した結果
/// @param argc 引数の数
/// @param argv 引数の配列
/// @param parser コマンドライン引数のパーサ
inline void ParseArguments(Options &options, int argc, char **argv, cmdline::parser &parser)
{
    parser.parse_check(argc, argv);

    auto filepathAsString = parser.get<std::string>("output");

    auto outputFilepath = std::filesystem::path(filepathAsString);
    options.OutputFilename.Value(outputFilepath);

    auto inputFilename = std::filesystem::path(parser.get<std::string>("input"));
    options.InputFilename.Value(inputFilename);

    options.LogLevel.Value(LogLevelFromString(parser.get<std::string>("log")));
}

/// @brief 出力オプションを検証する．
/// @param options 検証対象の Options オブジェクト
inline void ValidateOutputOption(const Options &options)
{
    auto filepath = options.OutputFilename.Value();
    if (filepath.empty())
    {
        SPDLOG_INFO("Output is disabled");
        return;
    }

    auto validExtensions = Options::FileTypeKeys();
    auto extension = options.OutputFilename.Value().extension();

    if (std::find(validExtensions.begin(), validExtensions.end(), extension) == validExtensions.end())
    {
        throw std::runtime_error("Invalid output file extension");
    }
}

/// @brief Options オブジェクトを検証する．
/// @param options 検証対象の Options オブジェクト
inline void ValidateOptions(const Options &options)
{
    std::vector<std::function<void(const Options &)>> validators = {ValidateOutputOption};
    std::for_each(validators.begin(), validators.end(), [&options](auto &validator) { validator(options); });
}

/// @brief 絶対時間から PcapPacketHeader を生成する．
/// @param absolute 絶対時間
/// @return 生成した PcapPacketHeader
/// @details PcapPacketHeader の IncludedLength と OriginalLength は 0 に設定される．
inline Packet::PcapPacketHeaderPtr CreatePcapPacketHeader(Packet::AbsolutePtr absolute)
{
    auto timestampNs = absolute->TimestampNs.Value();
    auto timestampSeconds = std::chrono::duration_cast<std::chrono::seconds>(timestampNs);
    auto timestampMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(timestampNs);

    auto pcapPacketHeader = std::make_shared<Packet::PcapPacketHeader>();
    pcapPacketHeader->TimestampSeconds.Value(timestampSeconds);
    pcapPacketHeader->TimestampMicroseconds.Value(timestampMicroseconds - timestampSeconds);
    pcapPacketHeader->IncludedLength.Value(0);
    pcapPacketHeader->OriginalLength.Value(0);

    return pcapPacketHeader;
}

/// @brief パケットを pcap ファイルに保存する．
/// @param outputPath 保存先のパス
/// @param packets 保存するパケット
inline void SaveAsPcap(std::filesystem::path outputPath, std::vector<Packet::StackablePtr> packets)
{
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_dumper_t *dumper;

    handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!handle)
    {
        throw std::runtime_error("pcap_open_dead failed");
    }

    dumper = pcap_dump_open(handle, outputPath.c_str());
    if (!dumper)
    {
        auto fmt = boost::format("pcap_dump_open failed: %1%");
        auto msg = fmt % pcap_geterr(handle);
        throw std::runtime_error(msg.str());
    }

    for (auto stackable : packets)
    {
        if (stackable == nullptr)
        {
            SPDLOG_WARN("The stackable is null.");
            continue;
        }

        // Stackable が Absolute であることを検証する
        auto absolute = std::dynamic_pointer_cast<Packet::Absolute>(stackable);
        if (absolute == nullptr)
        {
            SPDLOG_WARN("The stackable is not Absolute.");
            continue;
        }

        // Absolute を PcapPacketHeader に変換する
        auto pcapPacketHeader = CreatePcapPacketHeader(absolute);

        // Absolute がもつ Stackable を取得する
        auto stackableEntity = absolute->Stack.Value();
        if (stackableEntity == nullptr)
        {
            SPDLOG_WARN("The stackable entity is null.");
            continue;
        }
        auto composed = Packet::Stackable::Compose(stackableEntity);

        pcap_pkthdr header;
        header.len = composed->Length();
        header.caplen = composed->Length();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(absolute->TimestampNs.Value());
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(absolute->TimestampNs.Value());
        header.ts.tv_sec = seconds.count();
        header.ts.tv_usec = (microseconds - seconds).count();

        pcap_dump(reinterpret_cast<u_char *>(dumper), &header, composed->DataArray().get());
    }

    pcap_dump_close(dumper);
    pcap_close(handle);
}

/// @brief パケットを json ファイルに保存する．
/// @param outputPath 保存先のパス
/// @param packets 保存するパケット
inline void SaveAsJson(std::filesystem::path outputPath, std::vector<Packet::StackablePtr> packets)
{
    auto json = nlohmann::json::array();
    for (auto index = 0; index < packets.size(); index++)
    {
        auto packetToGetJson = std::shared_ptr<Packet::Stackable>(nullptr);
        auto &packet = packets[index];

        // packet が Absolute であることを検証する
        // TODO Absolute 型だけでなく，メタ情報を持つ Stackable であることを検証する．
        auto &stackablePacketRef = *packet;
        if (typeid(stackablePacketRef) == typeid(Packet::Absolute))
        {
            packetToGetJson = packet;
        }
        else
        {
            // メタ情報を持つ Stackable を持たない場合，強制的にメタ情報を付与する．
            SPDLOG_WARN("The stackable does not have the meta information such as timestamp.",
                        "This packet will added a meta information");
            auto pseudoAbsolute = std::make_shared<Packet::Absolute>();
            pseudoAbsolute->TimestampNs.Value(std::chrono::seconds(index));
            packetToGetJson = pseudoAbsolute;
        }

        auto stackableEntityPtr = packetToGetJson->StackableEntity();
        auto stackableEntityJson = stackableEntityPtr->ToJson();
        json.push_back(stackableEntityJson);
    }

    auto ofs = std::ofstream(outputPath);
    ofs << json;
}

#endif
