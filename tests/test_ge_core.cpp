#include "ge/core/asserts.h"
#include "ge/core/core.h"
#include "ge/core/log.h"
#include "ge/layer.h"
#include "ge/layer_stack.h"
#include "ge/window/key_event.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

class GECoreTest: public ::testing::Test
{
protected:
    void SetUp() override { GE::Log::initialize(); }
    void TearDown() override { GE::Log::shutdown(); }
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
    EXPECT_DEATH(GE_CORE_ASSERT(2 * 2 == 5, "core assert"), "");
    EXPECT_DEATH(GE_ASSERT(2 < 0, "client assert"), "");
#endif
    GE_CORE_ASSERT(true, "True =)");
    GE_ASSERT(2 * 2 == 4, "Yes");
}

class LayerMock: public GE::Layer
{
public:
    MOCK_METHOD(void, onAttach, (), (override));
    MOCK_METHOD(void, onDetach, (), (override));
    MOCK_METHOD(void, onUpdate, (), (override));
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

        EXPECT_CALL(*layer_1, onUpdate());
        EXPECT_CALL(*layer_2, onUpdate()).Times(0);
        EXPECT_CALL(*layer_3, onUpdate());
        EXPECT_CALL(*overlay_1, onUpdate());
        EXPECT_CALL(*overlay_2, onUpdate());
    }

    for (auto& layer : layer_stack) {
        layer->onUpdate();
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

        EXPECT_CALL(*overlay_2, onUpdate());
        EXPECT_CALL(*overlay_1, onUpdate());
        EXPECT_CALL(*layer_3, onUpdate());
        EXPECT_CALL(*layer_2, onUpdate()).Times(0);
        EXPECT_CALL(*layer_1, onUpdate());
    }

    for (auto layer = layer_stack.rbegin(); layer != layer_stack.rend(); ++layer) {
        (*layer)->onUpdate();
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

        EXPECT_CALL(*layer_1, onUpdate());
        EXPECT_CALL(*layer_2, onUpdate());
        EXPECT_CALL(*overlay_1, onUpdate()).Times(0);
        EXPECT_CALL(*overlay_2, onUpdate());
        EXPECT_CALL(*overlay_3, onUpdate());
    }

    for (auto& layer : layer_stack) {
        layer->onUpdate();
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

        EXPECT_CALL(*overlay_3, onUpdate());
        EXPECT_CALL(*overlay_2, onUpdate());
        EXPECT_CALL(*overlay_1, onUpdate()).Times(0);
        EXPECT_CALL(*layer_2, onUpdate());
        EXPECT_CALL(*layer_1, onUpdate());
    }

    for (auto layer = layer_stack.rbegin(); layer != layer_stack.rend(); ++layer) {
        (*layer)->onUpdate();
    }
}

} // namespace
