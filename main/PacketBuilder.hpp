#ifndef PACKET_BUILDER__PACKET_BUILDER_HPP__
#define PACKET_BUILDER__PACKET_BUILDER_HPP__

#include <NotifyProperty.hpp>
#include <algorithm>
#include <boost/format.hpp>
#include <cmdline.h>
#include <filesystem>
#include <spdlog/spdlog.h>

enum FileType
{
    Pcap,
    None
};

std::string FileTypeToString(FileType fileType)
{
    switch (fileType)
    {
    case FileType::Pcap:
        return "Pcap";
    case FileType::None:
        return "None";
    default:
        return "Unknown";
    }
}

class Options
{
  public:
    Options()
        : InputFilename(std::filesystem::path()), OutputFilename(std::filesystem::path()),
          OutputFileType(FileType::None), LogLevel(spdlog::level::info)
    {
        InputFilename.RegisterCallback([](std::filesystem::path oldPath, std::filesystem::path newPath) {
            if (std::filesystem::exists(newPath))
            {
                auto msgfmt = boost::format("Input file: %1%") % newPath.string();
                SPDLOG_INFO(msgfmt.str());
            }
            else
            {
                auto msgfmt = boost::format("Input file %1% does not exist") % newPath.string();
                throw std::runtime_error(msgfmt.str());
            }
        });

        OutputFilename.RegisterCallback([this](std::filesystem::path oldPath, std::filesystem::path newPath) {
            if (newPath.empty())
            {
                SPDLOG_INFO("Output is disabled");
                return;
            }

            if (std::filesystem::exists(newPath))
            {
                auto msgfmt = boost::format("Output file: %1%") % newPath.string();
                SPDLOG_INFO(msgfmt.str());

                auto extension = newPath.extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(),
                               [](unsigned char c) { return std::tolower(c); });

                std::map<std::string, FileType> fileTypeMap = {{".pcap", FileType::Pcap}};
                auto type = fileTypeMap.find(extension);
                this->OutputFileType.Value(type == fileTypeMap.end() ? FileType::None : type->second);
            }
            else
            {
                auto msgfmt = boost::format("Output file %1% does not exist") % newPath.string();
                throw std::runtime_error(msgfmt.str());
            }
        });

        OutputFileType.RegisterCallback([](FileType oldType, FileType newType) {
            auto msgfmt = boost::format("Output file type: %1%(%2%)") % newType % FileTypeToString(newType);
            SPDLOG_INFO(msgfmt.str());
        });
    };

    Utility::NotifyProperty<std::filesystem::path> InputFilename;
    Utility::NotifyProperty<std::filesystem::path> OutputFilename;
    Utility::NotifyProperty<FileType> OutputFileType;
    Utility::NotifyProperty<spdlog::level::level_enum> LogLevel;
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

#endif
