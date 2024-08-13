#include <PcapPacketHeader.hpp>
#include <PcapPacketHeaderEntity.hpp>

namespace Packet
{
PcapPacketHeader::PcapPacketHeader() : Stackable(HeaderSize, std::make_shared<PacketEntity::PcapPacketHeaderEntity>())
{
}

uint32_t PcapPacketHeader::TimestampSeconds() const
{
    return this->Header()->timestampSeconds;
}

void PcapPacketHeader::TimestampSeconds(uint32_t timestampSeconds)
{
    this->Header()->timestampSeconds = timestampSeconds;
}

uint32_t PcapPacketHeader::TimestampMicroseconds() const
{
    return this->Header()->timestampMicroseconds;
}

void PcapPacketHeader::TimestampMicroseconds(uint32_t timestampMicroseconds)
{
    this->Header()->timestampMicroseconds = timestampMicroseconds;
}

uint32_t PcapPacketHeader::IncludedLength() const
{
    return this->Header()->includedLength;
}

void PcapPacketHeader::IncludedLength(uint32_t includedLength)
{
    this->Header()->includedLength = includedLength;
}

uint32_t PcapPacketHeader::OriginalLength() const
{
    return this->Header()->originalLength;
}

void PcapPacketHeader::OriginalLength(uint32_t originalLength)
{
    this->Header()->originalLength = originalLength;
}

struct RecordHeader *PcapPacketHeader::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct RecordHeader *header = reinterpret_cast<struct RecordHeader *>(data_ptr);
    return header;
}

void PcapPacketHeader::OnStacked()
{
    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto totalLength = Stackable::GetTotalLength(this->Stack());
    IncludedLength(totalLength);
    OriginalLength(totalLength);
}
} // namespace Packet
