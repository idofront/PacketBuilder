#include <Options.hpp>
#include <boost/format.hpp>
#include <map>

/// @brief ファイルパスから FileType を取得する．
/// @param path ファイルパス
/// @return FileType
FileType FileTypeFromPath(std::filesystem::path path)
{
    auto extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    auto type = Options::FileTypeMap.find(extension);
    return type == Options::FileTypeMap.end() ? FileType::None : type->second;
}

Options::Options()
    : InputFilename(std::filesystem::path()), OutputFilename(std::filesystem::path()), OutputFileType(FileType::None),
      InputFileType(FileType::None), LogLevel(spdlog::level::info)
{
    InputFilename.RegisterCallback([this](std::filesystem::path oldPath, std::filesystem::path newPath) {
        if (std::filesystem::exists(newPath))
        {
            auto msgfmt = boost::format("Input file %1% exists.") % newPath.string();
            SPDLOG_INFO(msgfmt.str());
        }
        else
        {
            auto msgfmt = boost::format("Input file %1% does not exist.") % newPath.string();
            throw std::runtime_error(msgfmt.str());
        }

        auto type = FileTypeFromPath(newPath);
        this->InputFileType.Value(type);
    });

    InputFileType.RegisterCallback([](FileType oldType, FileType newType) {
        auto msgfmt = boost::format("Input file type: %1%(%2%)") % newType % FileTypeToString(newType);
        SPDLOG_INFO(msgfmt.str());
    });

    OutputFilename.RegisterCallback([this](std::filesystem::path oldPath, std::filesystem::path newPath) {
        if (newPath.empty())
        {
            SPDLOG_INFO("Output is disabled");
            return;
        }

        if (std::filesystem::exists(newPath))
        {
            auto msgfmt = boost::format("The output file %1% is already exist. Overwrite it.") % newPath.string();
            SPDLOG_INFO(msgfmt.str());
        }

        auto type = FileTypeFromPath(newPath);
        this->OutputFileType.Value(type);
    });

    OutputFileType.RegisterCallback([](FileType oldType, FileType newType) {
        auto msgfmt = boost::format("Output file type: %1%(%2%)") % newType % FileTypeToString(newType);
        SPDLOG_INFO(msgfmt.str());
    });
};

/// @brief FileTypeMap の初期値
const std::map<std::string, FileType> Options::FileTypeMap = {
    {".pcap", FileType::Pcap},
    {".json", FileType::Json},
};

/// @brief FileTypeMap のキーを取得する．
/// @return FileTypeMap のキー
std::vector<std::string> Options::FileTypeKeys()
{
    auto keys = std::vector<std::string>();
    std::transform(FileTypeMap.begin(), FileTypeMap.end(), std::back_inserter(keys),
                   [](std::pair<std::string, FileType> pair) { return pair.first; });
    return keys;
}
