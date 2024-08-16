#include <Options.hpp>
#include <boost/format.hpp>
#include <map>

Options::Options()
    : InputFilename(std::filesystem::path()), OutputFilename(std::filesystem::path()), OutputFileType(FileType::None),
      LogLevel(spdlog::level::info)
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
