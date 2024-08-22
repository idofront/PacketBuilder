#include <EntityService.hpp>
#include <PacketService.hpp>
#include <Utility.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <nameof/nameof.h>
#include <netinet/ether.h>

namespace
{

class SerializeEntityTest : public ::testing::Test
{
  protected:
    SerializeEntityTest()
    {
    }

    virtual ~SerializeEntityTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(SerializeEntityTest, Ethernet)
{
    auto ethernetEntity = std::make_shared<PacketEntity::EthernetEntity>();
    ethernetEntity->Type = ETHERTYPE_IP;
    ethernetEntity->SourceMac = "12:34:56:78:9A:BC";
    ethernetEntity->DestinationMac = "34:56:78:9A:BC:DE";
    auto actuallyJson = ethernetEntity->ToJson();

    auto expectJson = nlohmann::json();
    expectJson["EntityType"] = "PacketEntity::EthernetEntity";
    expectJson[nameof(ethernetEntity->Type)] = ETHERTYPE_IP;
    expectJson[nameof(ethernetEntity->SourceMac)] = "12:34:56:78:9A:BC";
    expectJson[nameof(ethernetEntity->DestinationMac)] = "34:56:78:9A:BC:DE";

    EXPECT_EQ(actuallyJson, expectJson);
}

TEST_F(SerializeEntityTest, Ipv4)
{
    auto ipv4Entity = std::make_shared<PacketEntity::Ipv4Entity>();
    ipv4Entity->Version = 4;
    ipv4Entity->IHL = 5;
    ipv4Entity->DSCP = 0;
    ipv4Entity->ECN = 0;
    ipv4Entity->TotalLength = 20;
    ipv4Entity->Identification = 0;
    ipv4Entity->Flags = 0;
    ipv4Entity->FragmentOffset = 0;
    ipv4Entity->TTL = 64;
    ipv4Entity->Protocol = 6;
    ipv4Entity->HeaderChecksum = 0xF00F;
    ipv4Entity->SourceAddress = "192.168.0.1";
    ipv4Entity->DestinationAddress = "172.16.1.1";
    auto actuallyJson = ipv4Entity->ToJson();

    auto expectJson = nlohmann::json();
    expectJson["EntityType"] = "PacketEntity::Ipv4Entity";
    expectJson[nameof(ipv4Entity->Version)] = 4;
    expectJson[nameof(ipv4Entity->IHL)] = 5;
    expectJson[nameof(ipv4Entity->DSCP)] = 0;
    expectJson[nameof(ipv4Entity->ECN)] = 0;
    expectJson[nameof(ipv4Entity->TotalLength)] = 20;
    expectJson[nameof(ipv4Entity->Identification)] = 0;
    expectJson[nameof(ipv4Entity->Flags)] = 0;
    expectJson[nameof(ipv4Entity->FragmentOffset)] = 0;
    expectJson[nameof(ipv4Entity->TTL)] = 64;
    expectJson[nameof(ipv4Entity->Protocol)] = 6;
    expectJson[nameof(ipv4Entity->HeaderChecksum)] = 0xF00F;
    expectJson[nameof(ipv4Entity->SourceAddress)] = "192.168.0.1";
    expectJson[nameof(ipv4Entity->DestinationAddress)] = "172.16.1.1";

    EXPECT_EQ(actuallyJson, expectJson);
}

TEST_F(SerializeEntityTest, Udp)
{
    auto udpEntity = std::make_shared<PacketEntity::UdpEntity>();
    udpEntity->SourcePort = 12345;
    udpEntity->DestinationPort = 54321;
    udpEntity->Length = 8;
    udpEntity->Checksum = 0xF00F;
    auto actuallyJson = udpEntity->ToJson();

    auto expectJson = nlohmann::json();
    expectJson["EntityType"] = "PacketEntity::UdpEntity";
    expectJson[nameof(udpEntity->SourcePort)] = 12345;
    expectJson[nameof(udpEntity->DestinationPort)] = 54321;
    expectJson[nameof(udpEntity->Length)] = 8;
    expectJson[nameof(udpEntity->Checksum)] = 0xF00F;

    EXPECT_EQ(actuallyJson, expectJson);
}

TEST_F(SerializeEntityTest, Binary)
{
    auto binaryEntity = std::make_shared<PacketEntity::BinaryEntity>(0x05);
    auto data = std::vector<std::uint8_t>({0x00, 0x01, 0x02, 0x03, 0x04});
    std::copy(data.begin(), data.end(), binaryEntity->Data->begin());

    auto actuallyJson = binaryEntity->ToJson();

    auto expectJson = nlohmann::json();
    expectJson["EntityType"] = "PacketEntity::BinaryEntity";
    expectJson[nameof(binaryEntity->Data)] = {0x00, 0x01, 0x02, 0x03, 0x04};

    EXPECT_EQ(actuallyJson, expectJson);
}

TEST_F(SerializeEntityTest, Stacked)
{
    auto binaryEntity = std::make_shared<PacketEntity::BinaryEntity>(0x05);
    auto data = std::vector<std::uint8_t>({0x00, 0x01, 0x02, 0x03, 0x04});
    std::copy(data.begin(), data.end(), binaryEntity->Data->begin());

    auto udpEntity = std::make_shared<PacketEntity::UdpEntity>();
    udpEntity->SourcePort = 12345;
    udpEntity->DestinationPort = 54321;
    udpEntity->Length = 8;
    udpEntity->Checksum = 0xF00F;
    udpEntity->Stack.Value(binaryEntity);

    auto ipv4Entity = std::make_shared<PacketEntity::Ipv4Entity>();
    ipv4Entity->Version = 4;
    ipv4Entity->IHL = 5;
    ipv4Entity->DSCP = 0;
    ipv4Entity->ECN = 0;
    ipv4Entity->TotalLength = 20;
    ipv4Entity->Identification = 0;
    ipv4Entity->Flags = 0;
    ipv4Entity->FragmentOffset = 0;
    ipv4Entity->TTL = 64;
    ipv4Entity->Protocol = 6;
    ipv4Entity->HeaderChecksum = 0xF00F;
    ipv4Entity->SourceAddress = "192.168.0.1";
    ipv4Entity->DestinationAddress = "172.16.1.1";
    ipv4Entity->Stack.Value(udpEntity);

    auto ethernetEntity = std::make_shared<PacketEntity::EthernetEntity>();
    ethernetEntity->Type = ETHERTYPE_IP;
    ethernetEntity->SourceMac = "12:34:56:78:9A:BC";
    ethernetEntity->DestinationMac = "34:56:78:9A:BC:DE";
    ethernetEntity->Stack.Value(ipv4Entity);

    auto actuallyJson = ethernetEntity->ToJson();

    auto expectBinaryJson = nlohmann::json();
    expectBinaryJson["EntityType"] = "PacketEntity::BinaryEntity";
    expectBinaryJson[nameof(binaryEntity->Data)] = {0x00, 0x01, 0x02, 0x03, 0x04};

    auto expectUdpJson = nlohmann::json();
    expectUdpJson["EntityType"] = "PacketEntity::UdpEntity";
    expectUdpJson[nameof(udpEntity->SourcePort)] = 12345;
    expectUdpJson[nameof(udpEntity->DestinationPort)] = 54321;
    expectUdpJson[nameof(udpEntity->Length)] = 8;
    expectUdpJson[nameof(udpEntity->Checksum)] = 0xF00F;
    expectUdpJson[nameof(udpEntity->Stack)] = expectBinaryJson;

    auto expectIpv4Json = nlohmann::json();
    expectIpv4Json["EntityType"] = "PacketEntity::Ipv4Entity";
    expectIpv4Json[nameof(ipv4Entity->Version)] = 4;
    expectIpv4Json[nameof(ipv4Entity->IHL)] = 5;
    expectIpv4Json[nameof(ipv4Entity->DSCP)] = 0;
    expectIpv4Json[nameof(ipv4Entity->ECN)] = 0;
    expectIpv4Json[nameof(ipv4Entity->TotalLength)] = 20;
    expectIpv4Json[nameof(ipv4Entity->Identification)] = 0;
    expectIpv4Json[nameof(ipv4Entity->Flags)] = 0;
    expectIpv4Json[nameof(ipv4Entity->FragmentOffset)] = 0;
    expectIpv4Json[nameof(ipv4Entity->TTL)] = 64;
    expectIpv4Json[nameof(ipv4Entity->Protocol)] = 6;
    expectIpv4Json[nameof(ipv4Entity->HeaderChecksum)] = 0xF00F;
    expectIpv4Json[nameof(ipv4Entity->SourceAddress)] = "192.168.0.1";
    expectIpv4Json[nameof(ipv4Entity->DestinationAddress)] = "172.16.1.1";
    expectIpv4Json[nameof(ipv4Entity->Stack)] = expectUdpJson;

    auto expectEthernetJson = nlohmann::json();
    expectEthernetJson["EntityType"] = "PacketEntity::EthernetEntity";
    expectEthernetJson[nameof(ethernetEntity->Type)] = ETHERTYPE_IP;
    expectEthernetJson[nameof(ethernetEntity->SourceMac)] = "12:34:56:78:9A:BC";
    expectEthernetJson[nameof(ethernetEntity->DestinationMac)] = "34:56:78:9A:BC:DE";
    expectEthernetJson[nameof(ethernetEntity->Stack)] = expectIpv4Json;

    EXPECT_EQ(actuallyJson, expectEthernetJson);
}

} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
