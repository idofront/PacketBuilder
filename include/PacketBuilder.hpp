#ifndef PACKET_BUILDER__PACKET_BUILDER_HPP__
#define PACKET_BUILDER__PACKET_BUILDER_HPP__

#include <Absolute.hpp>
#include <NotifyProperty.hpp>
#include <Options.hpp>
#include <PcapPacketHeader.hpp>
#include <algorithm>
#include <boost/format.hpp>
#include <cmdline/cmdline.h>
#include <filesystem>
#include <spdlog/spdlog.h>

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

inline void ValidateOutputOption(const Options &options)
{
    auto filepath = options.OutputFilename.Value();
    if (filepath.empty())
    {
        SPDLOG_INFO("Output is disabled");
        return;
    }

    auto validExtensions = {".pcap"};
    auto extension = options.OutputFilename.Value().extension();

    if (std::find(validExtensions.begin(), validExtensions.end(), extension) == validExtensions.end())
    {
        throw std::runtime_error("Invalid output file extension");
    }
}

inline void ValidateOptions(const Options &options)
{
    std::vector<std::function<void(const Options &)>> validators = {ValidateOutputOption};
    std::for_each(validators.begin(), validators.end(), [&options](auto &validator) { validator(options); });
}

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

#endif
