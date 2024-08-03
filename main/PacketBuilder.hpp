#ifndef PACKET_BUILDER__PACKET_BUILDER_HPP__
#define PACKET_BUILDER__PACKET_BUILDER_HPP__

#include <Utility/Utility.hpp>
#include <algorithm>
#include <cmdline.h>
#include <filesystem>

enum FileType
{
    Pcap,
    None
};
class Options
{
  public:
    Options() = default;
    std::filesystem::path OutputFilename() const
    {
        return _OutputFilename;
    }

    void OutputFilename(std::filesystem::path outputFilename)
    {
        _OutputFilename = outputFilename;
    }

    FileType OutputFileType() const
    {
        auto extension = _OutputFilename.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        std::map<std::string, FileType> fileTypeMap = {{".pcap", FileType::Pcap}};
        auto type = fileTypeMap.find(extension);
        return type == fileTypeMap.end() ? FileType::None : type->second;
    }

    spdlog::level::level_enum LogLevel() const
    {
        return _LogLevel;
    }

    void LogLevel(spdlog::level::level_enum logLevel)
    {
        _LogLevel = logLevel;
    }

  private:
    std::filesystem::path _OutputFilename;
    spdlog::level::level_enum _LogLevel;
};

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

    // Log level.
    parser.add<std::string>("log", 'l', "Log level", false, "info",
                            cmdline::oneof<std::string>("trace", "debug", "info", "warn", "error", "critical"));

    return parser;
}

inline void ParseArguments(Options &options, int argc, char **argv, cmdline::parser &parser)
{
    parser.parse_check(argc, argv);

    auto filepathAsString = parser.get<std::string>("output");

    auto filepath = std::filesystem::path(filepathAsString);
    options.OutputFilename(filepath);

    options.LogLevel(LogLevelFromString(parser.get<std::string>("log")));
}

inline void ValidateOutputOption(const Options &options)
{
    auto filepath = options.OutputFilename();
    if (filepath.empty())
    {
        SPDLOG_INFO("Output is disabled");
        return;
    }

    auto validExtensions = {".pcap"};
    auto extension = options.OutputFilename().extension();

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

#endif
