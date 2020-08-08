#include "ge/core/asserts.h"
#include "ge/core/core.h"
#include "ge/core/log.h"
#include "ge/core/timestamp.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"
#include "ge/layer.h"
#include "ge/layer_stack.h"
#include "ge/window/key_event.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

class GECoreTest: public ::testing::Test
{
protected:
    void SetUp() override { ASSERT_TRUE(GE::Log::get()->initialize()); }
    void TearDown() override { GE::Log::get()->shutdown(); }
};

TEST_F(GECoreTest, CoreLogger)
{
    GE_CORE_TRACE("{}", "trace");
    GE_CORE_DBG("{1} {0}", "debug", "^^");
    GE_CORE_INFO("info {1}={0}", SPDLOG_ACTIVE_LEVEL, "log level");
    GE_CORE_WARN("warn");
    GE_CORE_ERR("error: {}, {}", "one", 2);
    GE_CORE_CRIT("crit =(");
}

TEST_F(GECoreTest, ClientLogger)
{
    GE_TRACE("{}", "trace");
    GE_DBG("{1} {0}", "debug", "^^");
    GE_INFO("info {1}={0}", SPDLOG_ACTIVE_LEVEL, "log level");
    GE_WARN("warn");
    GE_ERR("error: {}, {}", "one", 2);
    GE_CRIT("crit =(");
}

TEST_F(GECoreTest, Asserts)
{
#if !defined(GE_DISABLE_ASSERTS)
    EXPECT_DEATH(GE_CORE_ASSERT_MSG(0 == 1, "core assert: {}", 1), "");
    EXPECT_DEATH(GE_CORE_ASSERT(0 == 2), "");
    EXPECT_DEATH(GE_ASSERT_MSG(0 == 3, "client assert: {}", 2), "");
    EXPECT_DEATH(GE_ASSERT(0 == 4), "");
#endif
    GE_CORE_ASSERT_MSG(1 == 1, "core assert: {}", 3);
    GE_CORE_ASSERT(2 == 2);
    GE_ASSERT_MSG(3 == 3, "client assert: {}", 4);
    GE_ASSERT(4 == 4);
}

TEST_F(GECoreTest, Profiler)
{
    const char* profile_log = "profile_test.json";

    auto profile_writer = [](const char* name) {
        for (uint32_t i{0}; i < 10; i++) {
            GE_PROFILE_SCOPE(name);
            GE::sleep(0.001);
        }
    };

    auto write_profile = [&profile_writer](const char* name) {
        constexpr uint8_t thread_count{10};
        std::vector<std::thread> threads;
        threads.reserve(thread_count);

        for (uint32_t i{0}; i < thread_count; i++) {
            threads.emplace_back(profile_writer, name);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    };

    write_profile("BeforeSession");
    GE_PROFILE_ENABLE(true);
    GE_PROFILE_BEGIN_SESSION("ProfileTest", profile_log);
    write_profile("InsideSession");
    GE_PROFILE_END_SESSION();
    write_profile("AfterSession");

    std::remove(profile_log);
}

TEST(TimestampTest, Conversion)
{
    GE::Timestamp ts{0.123456789};

    EXPECT_DOUBLE_EQ(ts.sec(), 0.123456789);
    EXPECT_DOUBLE_EQ(ts.ms(), 123.456789);
    EXPECT_DOUBLE_EQ(ts.us(), 123456.789);
    EXPECT_DOUBLE_EQ(ts.ns(), 123456789.0);
}

TEST(TimestampTest, Addition)
{
    GE::Timestamp time_one{0.123456789};
    GE::Timestamp time_two{0.987654321};
    GE::Timestamp sum = time_one + time_two;

    EXPECT_DOUBLE_EQ(sum.sec(), 1.11111111);
    EXPECT_DOUBLE_EQ(sum.ms(), 1111.11111);
    EXPECT_DOUBLE_EQ(sum.us(), 1111111.11);
    EXPECT_DOUBLE_EQ(sum.ns(), 1111111110.0);
}

TEST(TimestampTest, FetchAdd)
{
    GE::Timestamp timestamp{0.123456789};
    timestamp += 0.987654321;

    EXPECT_DOUBLE_EQ(timestamp.sec(), 1.11111111);
    EXPECT_DOUBLE_EQ(timestamp.ms(), 1111.11111);
    EXPECT_DOUBLE_EQ(timestamp.us(), 1111111.11);
    EXPECT_DOUBLE_EQ(timestamp.ns(), 1111111110.0);
}

TEST(TimestampTest, Subtraction)
{
    GE::Timestamp start{0.987654321};
    GE::Timestamp end{1.11111111};
    GE::Timestamp duration = end - start;

    EXPECT_DOUBLE_EQ(duration.sec(), 0.123456789);
    EXPECT_DOUBLE_EQ(duration.ms(), 123.456789);
    EXPECT_DOUBLE_EQ(duration.us(), 123456.789);
    EXPECT_DOUBLE_EQ(duration.ns(), 123456789.0);
}

TEST(TimestampTest, FetchSub)
{
    GE::Timestamp timestamp{1.11111111};
    timestamp -= 0.987654321;

    EXPECT_DOUBLE_EQ(timestamp.sec(), 0.123456789);
    EXPECT_DOUBLE_EQ(timestamp.ms(), 123.456789);
    EXPECT_DOUBLE_EQ(timestamp.us(), 123456.789);
    EXPECT_DOUBLE_EQ(timestamp.ns(), 123456789.0);
}

class LayerMock: public GE::Layer
{
public:
    MOCK_METHOD(void, onAttach, (), (override));
    MOCK_METHOD(void, onDetach, (), (override));
    MOCK_METHOD(void, onUpdate, (GE::Timestamp delta_time), (override));
    MOCK_METHOD(void, onEvent, (GE::Event * event), (override));
};

class LayerStackTest: public ::testing::Test
{
protected:
    void SetUp() override
    {
        layer_1 = GE::makeShared<LayerMock>();
        layer_2 = GE::makeShared<LayerMock>();
        overlay_1 = GE::makeShared<LayerMock>();
        overlay_2 = GE::makeShared<LayerMock>();

        layer_stack.pushLayer(layer_1);
        layer_stack.pushOverlay(overlay_1);
        layer_stack.pushLayer(layer_2);
        layer_stack.pushOverlay(overlay_2);

        EXPECT_CALL(*layer_1, onDetach());
        EXPECT_CALL(*layer_2, onDetach());
        EXPECT_CALL(*overlay_1, onDetach());
        EXPECT_CALL(*overlay_2, onDetach());
    }

    GE::LayerStack layer_stack;

    GE::Shared<LayerMock> layer_1;
    GE::Shared<LayerMock> layer_2;
    GE::Shared<LayerMock> overlay_1;
    GE::Shared<LayerMock> overlay_2;
};

TEST_F(LayerStackTest, ReplaceLayerForwardOrder)
{
    auto layer_3 = GE::makeShared<LayerMock>();

    EXPECT_CALL(*layer_3, onDetach());

    layer_stack.popLayer(layer_2);
    layer_stack.pushLayer(layer_3);

    {
        testing::InSequence seq;

        EXPECT_CALL(*layer_1, onUpdate({0.0}));
        EXPECT_CALL(*layer_2, onUpdate({0.0})).Times(0);
        EXPECT_CALL(*layer_3, onUpdate({0.0}));
        EXPECT_CALL(*overlay_1, onUpdate({0.0}));
        EXPECT_CALL(*overlay_2, onUpdate({0.0}));
    }

    for (auto& layer : layer_stack) {
        layer->onUpdate(0.0);
    }
}

TEST_F(LayerStackTest, ReplaceLayerReverseOrder)
{
    auto layer_3 = GE::makeShared<LayerMock>();

    EXPECT_CALL(*layer_3, onDetach());

    layer_stack.popLayer(layer_2);
    layer_stack.pushLayer(layer_3);

    {
        testing::InSequence seq;

        EXPECT_CALL(*overlay_2, onUpdate({0.0}));
        EXPECT_CALL(*overlay_1, onUpdate({0.0}));
        EXPECT_CALL(*layer_3, onUpdate({0.0}));
        EXPECT_CALL(*layer_2, onUpdate({0.0})).Times(0);
        EXPECT_CALL(*layer_1, onUpdate({0.0}));
    }

    for (auto layer = layer_stack.rbegin(); layer != layer_stack.rend(); ++layer) {
        (*layer)->onUpdate(0.0);
    }
}

TEST_F(LayerStackTest, ReplaceOverlayForwardOrder)
{
    auto overlay_3 = GE::makeShared<LayerMock>();

    EXPECT_CALL(*overlay_3, onDetach());

    layer_stack.popOverlay(overlay_1);
    layer_stack.pushOverlay(overlay_3);

    {
        testing::InSequence seq;

        EXPECT_CALL(*layer_1, onUpdate({0.0}));
        EXPECT_CALL(*layer_2, onUpdate({0.0}));
        EXPECT_CALL(*overlay_1, onUpdate({0.0})).Times(0);
        EXPECT_CALL(*overlay_2, onUpdate({0.0}));
        EXPECT_CALL(*overlay_3, onUpdate({0.0}));
    }

    for (auto& layer : layer_stack) {
        layer->onUpdate(0.0);
    }
}

TEST_F(LayerStackTest, ReplaceOverlayReverseOrder)
{
    auto overlay_3 = GE::makeShared<LayerMock>();

    EXPECT_CALL(*overlay_3, onDetach());

    layer_stack.popOverlay(overlay_1);
    layer_stack.pushOverlay(overlay_3);

    {
        testing::InSequence seq;

        EXPECT_CALL(*overlay_3, onUpdate({0.0}));
        EXPECT_CALL(*overlay_2, onUpdate({0.0}));
        EXPECT_CALL(*overlay_1, onUpdate({0.0})).Times(0);
        EXPECT_CALL(*layer_2, onUpdate({0.0}));
        EXPECT_CALL(*layer_1, onUpdate({0.0}));
    }

    for (auto layer = layer_stack.rbegin(); layer != layer_stack.rend(); ++layer) {
        (*layer)->onUpdate(0.0);
    }
}

} // namespace
