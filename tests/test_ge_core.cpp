#include <ge/ge.h>

#include "gtest/gtest.h"

namespace {

class TestGECore: public ::testing::Test
{
protected:
    void SetUp() override { GE_INITIALIZE(); }
    void TearDown() override { GE_SHUTDOWN(); }
};

TEST_F(TestGECore, CoreLogger)
{
    GE_CORE_TRACE("{}", "trace");
    GE_CORE_DBG("{1} {0}", "debug", "^^");
    GE_CORE_INFO("info {1}={0}", SPDLOG_ACTIVE_LEVEL, "log level");
    GE_CORE_WARN("warn");
    GE_CORE_ERR("error: {}, {}", "one", 2);
    GE_CORE_CRIT("crit =(");
}

TEST_F(TestGECore, ClientLogger)
{
    GE_TRACE("{}", "trace");
    GE_DBG("{1} {0}", "debug", "^^");
    GE_INFO("info {1}={0}", SPDLOG_ACTIVE_LEVEL, "log level");
    GE_WARN("warn");
    GE_ERR("error: {}, {}", "one", 2);
    GE_CRIT("crit =(");
}

TEST_F(TestGECore, Asserts)
{
    EXPECT_DEATH(GE_CORE_ASSERT(2 * 2 == 5, "core assert"), "");
    EXPECT_DEATH(GE_ASSERT(2 < 0, "client assert"), "");
    GE_CORE_ASSERT(true, "True =)");
    GE_ASSERT(2 * 2 == 4, "Yes");
}

} // namespace
