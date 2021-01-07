/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, Dmitry Shilnenkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ge/ecs/camera_controller_script.h"
#include "ge/ecs/scene.h"
#include "ge/ecs/scene_serializer.h"
#include "ge/renderer/colors.h"

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr auto TMP_DIR = "_tmp";

class SceneSerializerTest: public ::testing::Test
{
protected:
    GE::Scene scene;
    GE::Scene expected_scene;

    void SetUp() override;

    static void SetUpTestSuite();
    static void TearDownTestSuite();
};

void SceneSerializerTest::SetUp()
{
    auto square = expected_scene.createEntity("Square");
    auto& sprite_renderer = square.addComponent<GE::SpriteRendererComponent>();
    sprite_renderer.color = GE::Color::GREEN;

    auto camera = expected_scene.createEntity("Main Camera");
    camera.addComponent<GE::NativeScriptComponent>().bind<GE::CameraControllerScript>(
        camera);
}

void SceneSerializerTest::SetUpTestSuite()
{
    std::filesystem::create_directory(TMP_DIR);
}

void SceneSerializerTest::TearDownTestSuite()
{
    std::filesystem::remove_all(TMP_DIR);
}

TEST_F(SceneSerializerTest, serialization)
{
    auto scene_file = std::filesystem::path(TMP_DIR).append("scene.ge").string();
    ASSERT_TRUE(GE::SceneSerializer::serialize(scene_file, &expected_scene));
}

TEST_F(SceneSerializerTest, deserialization)
{
    static constexpr auto filename = "tests/data/scene-serializer/scene.ge";
    ASSERT_TRUE(GE::SceneSerializer::deserialize(filename, &scene));
}

} // namespace
