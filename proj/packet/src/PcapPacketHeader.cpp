#include <PcapPacketHeader.hpp>
#include <PcapPacketHeaderEntity.hpp>

namespace Packet
{
PcapPacketHeader::PcapPacketHeader()
    : Stackable(HeaderSize, std::make_shared<PacketEntity::PcapPacketHeaderEntity>()),
      TimestampSeconds(std::chrono::seconds(0)), TimestampMicroseconds(std::chrono::microseconds(0)), IncludedLength(0),
      OriginalLength(0)
{
    RegisterCallbacks();
}

struct RecordHeader *PcapPacketHeader::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct RecordHeader *header = reinterpret_cast<struct RecordHeader *>(data_ptr);
    return header;
}

void PcapPacketHeader::OnStacked(StackablePtr oldStackable, StackablePtr newStackable)
{
    Stackable::OnStacked(oldStackable, newStackable);

    SPDLOG_TRACE("{}", __PRETTY_FUNCTION__);
    auto totalLength = Stackable::GetTotalLength(this->Stack.Value());
    IncludedLength.Value(totalLength);
    OriginalLength.Value(totalLength);
}

PacketEntity::PcapPacketHeaderEntityPtr PcapPacketHeader::Entity()
{
    return std::dynamic_pointer_cast<PacketEntity::PcapPacketHeaderEntity>(this->StackableEntity());
}

void PcapPacketHeader::RegisterCallbacks()
{
    this->TimestampSeconds.RegisterCallback([this](std::chrono::seconds oldValue, std::chrono::seconds newValue) {
        auto timestampSeconds = this->TimestampSeconds.Value().count();
        Header()->timestampSeconds = timestampSeconds;
        auto timestampMicroseconds = this->TimestampMicroseconds.Value().count();

        Entity()->Seconds = timestampSeconds;
        Entity()->Microseconds = timestampMicroseconds;
    });

    this->TimestampMicroseconds.RegisterCallback(
        [this](std::chrono::microseconds oldValue, std::chrono::microseconds newValue) {
            auto timestampSeconds = this->TimestampSeconds.Value().count();
            auto timestampMicroseconds = this->TimestampMicroseconds.Value().count();
            Header()->timestampMicroseconds = timestampMicroseconds;

            Entity()->Seconds = timestampSeconds;
            Entity()->Microseconds = timestampMicroseconds;
        });

    this->IncludedLength.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        Header()->includedLength = newValue;
        Entity()->IncludedLength = newValue;
    });

    this->OriginalLength.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        Header()->originalLength = newValue;
        Entity()->OriginalLength = newValue;
    });
}
} // namespace Packet
