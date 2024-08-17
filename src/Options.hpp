#ifndef PACKET_BUILDER__OPTIONS_HPP
#define PACKET_BUILDER__OPTIONS_HPP

#include <FileType.hpp>
#include <NotifyProperty.hpp>
#include <cmdline/cmdline.h>
#include <filesystem>
#include <spdlog/spdlog.h>

/// @brief PacketBuilder が提供するコマンドライン引数を表す．
class Options
{
  public:
    Options();
    Utility::NotifyProperty<std::filesystem::path> InputFilename;
    Utility::NotifyProperty<std::filesystem::path> OutputFilename;
    Utility::NotifyProperty<FileType> OutputFileType;
    Utility::NotifyProperty<spdlog::level::level_enum> LogLevel;
};

#endif
