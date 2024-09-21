#include <EntityService.hpp>
#include <PacketService.hpp>
#include <Utility.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <nameof/nameof.h>
#include <netinet/ether.h>

namespace
{

class CreateAbsoluteEntityTest : public ::testing::Test
{
  protected:
    CreateAbsoluteEntityTest()
    {
    }

    virtual ~CreateAbsoluteEntityTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(CreateAbsoluteEntityTest, Default)
{
    auto absoluteEntityPtr = std::make_shared<PacketEntity::AbsoluteEntity>();

    // デフォルト値が設定されていることを確認する．
    auto expectTimestampNs = 0;
    ASSERT_EQ(absoluteEntityPtr->TimestampNs, expectTimestampNs);
}

TEST_F(CreateAbsoluteEntityTest, DefaultToJsonShould)
{
    auto absoluteEntityPtr = std::make_shared<PacketEntity::AbsoluteEntity>();

    // デフォルト値が設定されていることを確認する．
    auto expectTimestampNs = 0;
    ASSERT_EQ(absoluteEntityPtr->TimestampNs, expectTimestampNs);

    // JSONに変換する．
    {
        auto actualJson = absoluteEntityPtr->ToJson();

        // JSONの内容を確認する．
        auto expectJson = nlohmann::json::object();
        expectJson["EntityType"] = Utility::Demangle(typeid(PacketEntity::AbsoluteEntity).name());
        expectJson["TimestampNs"] = expectTimestampNs;
        ASSERT_EQ(actualJson, expectJson);
    }
}

TEST_F(CreateAbsoluteEntityTest, ToJsonShould)
{
    auto absoluteEntityPtr = std::make_shared<PacketEntity::AbsoluteEntity>();

    // デフォルト値が設定されていることを確認する．
    auto expectTimestampNs = 0xF1F2F3F4F5F6F7F8;
    absoluteEntityPtr->TimestampNs = expectTimestampNs;

    // JSONに変換する．
    {
        auto actualJson = absoluteEntityPtr->ToJson();

        // JSONの内容を確認する．
        auto expectJson = nlohmann::json::object();
        expectJson["EntityType"] = Utility::Demangle(typeid(PacketEntity::AbsoluteEntity).name());
        expectJson["TimestampNs"] = expectTimestampNs;
        ASSERT_EQ(actualJson, expectJson);
    }
}

TEST_F(CreateAbsoluteEntityTest, FromJsonShould)
{
    // JSONを作成する．
    auto json = R"({"TimestampNs":1000})"_json;

    // JSONからエンティティを作成する．
    auto absoluteEntityPtr =
        std::dynamic_pointer_cast<PacketEntity::AbsoluteEntity>(PacketEntity::AbsoluteEntity::FromJson(json));

    // エンティティの内容を確認する．
    auto expectTimestampNs = 1000;
    ASSERT_EQ(absoluteEntityPtr->TimestampNs, expectTimestampNs);
}
} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
