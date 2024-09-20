#ifndef PACKET_BUILDER__OPTIONS_HPP
#define PACKET_BUILDER__OPTIONS_HPP

#include <FileType.hpp>
#include <ObservableProperty.hpp>
#include <cmdline/cmdline.h>
#include <filesystem>
#include <spdlog/spdlog.h>

/// @brief PacketBuilder が提供するコマンドライン引数を表す．
class Options
{
  public:
    Options();
    Utility::ObservableProperty<std::filesystem::path> InputFilename;
    Utility::ObservableProperty<std::filesystem::path> OutputFilename;
    Utility::ObservableProperty<FileType> InputFileType;
    Utility::ObservableProperty<FileType> OutputFileType;
    Utility::ObservableProperty<spdlog::level::level_enum> LogLevel;

    static const std::map<std::string, FileType> FileTypeMap;
    static std::vector<std::string> FileTypeKeys();
};

#endif
