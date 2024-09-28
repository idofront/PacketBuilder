#include <EntityService.hpp>
#include <PacketService.hpp>
#include <Utility.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <nameof/nameof.h>
#include <netinet/ether.h>
#include <random>

class ParseBinaryTest : public ::testing::Test
{
  protected:
    ParseBinaryTest()
    {
    }

    virtual ~ParseBinaryTest()
    {
    }

    virtual void SetUp()
    {
        // シード値を固定することで，テストの再現性を担保する．
        mt = std::mt19937(0);
    }

    virtual void TearDown()
    {
    }

    PacketEntity::BinaryEntityPtr ParseBinary(const uint8_t *packet, std::size_t length)
    {
        auto entity = std::make_shared<PacketEntity::BinaryEntity>(length);
        for (std::size_t i = 0; i < length; i++)
        {
            entity->Data->at(i) = packet[i];
        }
        return entity;
    }

    std::mt19937 mt;
};

TEST_F(ParseBinaryTest, ParseBinaryTest)
{
    auto length = 64;
    auto randomsPtr = Utility::DataArrayPtr(new uint8_t[length]);
    for (std::size_t i = 0; i < length; i++)
    {
        randomsPtr.get()[i] = this->mt() % 256;
    }

    {
        auto dump = Utility::HexDump(randomsPtr, length);
        auto fmt = boost::format("Expect: %1%");
        auto msg = boost::str(fmt % dump);
        SPDLOG_INFO(msg);
    }

    auto entityPtr = this->ParseBinary(randomsPtr.get(), length);
    {
        auto buffLength = entityPtr->Data->size();
        auto buff = Utility::DataArrayPtr(new uint8_t[buffLength]);
        for (std::size_t i = 0; i < entityPtr->Data->size(); i++)
        {
            buff.get()[i] = entityPtr->Data->at(i);
        }

        auto dump = Utility::HexDump(buff, buffLength);
        auto fmt = boost::format("Actual: %1%");
        auto msg = boost::str(fmt % dump);
        SPDLOG_INFO(msg);
    }

    EXPECT_EQ(entityPtr->Stack.Value(), nullptr);
    EXPECT_EQ(entityPtr->Data->size(), length);

    for (std::size_t i = 0; i < length; i++)
    {
        auto expect = randomsPtr[i];
        auto actual = entityPtr->Data->at(i);
        EXPECT_EQ(actual, expect);
    }
}
