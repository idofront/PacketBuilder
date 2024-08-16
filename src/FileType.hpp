#ifndef PACKET_BUILDER__FILE_TYPE_HPP
#define PACKET_BUILDER__FILE_TYPE_HPP

#include <string>

enum FileType
{
    Pcap,
    None
};

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
