#include <PcapFileHeader.hpp>
#include <PcapFileHeaderEntity.hpp>

namespace Packet
{
PcapFileHeader::PcapFileHeader()
    : Stackable(HeaderSize, std::make_shared<PacketEntity::PcapFileHeaderEntity>()), MagicNumber(0), VersionMajor(0),
      VersionMinor(0), ThisZone(0), SigFigs(0), SnapLen(0), LinkType(0)
{
    RegisterCallbacks();

    this->MagicNumber.Value(0xa1b2c3d4);
    this->VersionMajor.Value(2);
    this->VersionMinor.Value(4);
    this->ThisZone.Value(0);
    this->SigFigs.Value(0);
    this->SnapLen.Value(65535);
    this->LinkType.Value(1);
}

struct pcap_file_header *PcapFileHeader::Header() const
{
    auto data_ptr = this->DataArray().get();
    struct pcap_file_header *header = reinterpret_cast<struct pcap_file_header *>(data_ptr);
    return header;
}

PacketEntity::PcapFileHeaderEntityPtr PcapFileHeader::Entity()
{
    return std::dynamic_pointer_cast<PacketEntity::PcapFileHeaderEntity>(this->StackableEntity());
}

void PcapFileHeader::RegisterCallbacks()
{
    this->MagicNumber.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        auto header = this->Header();
        header->magic = newValue;

        auto entity = this->Entity();
        entity->MagicNumber = newValue;
    });

    this->VersionMajor.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto header = this->Header();
        header->version_major = newValue;

        auto entity = this->Entity();
        entity->MajorVersion = newValue;
    });

    this->VersionMinor.RegisterCallback([this](uint16_t oldValue, uint16_t newValue) {
        auto header = this->Header();
        header->version_minor = newValue;

        auto entity = this->Entity();
        entity->MinorVersion = newValue;
    });

    this->ThisZone.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        auto header = this->Header();
        header->thiszone = newValue;

        auto entity = this->Entity();
        entity->ThisZone = newValue;
    });

    this->SigFigs.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        auto header = this->Header();
        header->sigfigs = newValue;

        auto entity = this->Entity();
        entity->SigFigs = newValue;
    });

    this->SnapLen.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        auto header = this->Header();
        header->snaplen = newValue;

        auto entity = this->Entity();
        entity->SnapLen = newValue;
    });

    this->LinkType.RegisterCallback([this](uint32_t oldValue, uint32_t newValue) {
        auto header = this->Header();
        header->linktype = newValue;

        auto entity = this->Entity();
        entity->Network = newValue;
    });
}
} // namespace Packet
