#ifndef PACKET_BUILDER__FILE_TYPE_HPP
#define PACKET_BUILDER__FILE_TYPE_HPP

#include <string>

/// @brief PacketBuilder が対応するファイル種別を表す．
enum FileType
{
    Pcap,
    None
};

/// @brief ファイル種別を文字列に変換する．
inline std::string FileTypeToString(FileType fileType)
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

#endif
