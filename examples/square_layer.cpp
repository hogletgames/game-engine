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

#include "square_layer.h"

#include "ge/debug/profile.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <array>

#define VERT_PER_QUAD   4
#define VERT_ELEM_COUNT 5
#define IND_PER_QUAD    6

#define FLAT_COLOR_VERT   "examples/assets/shaders/flat_color.vert"
#define FLAT_COLOR_FRAG   "examples/assets/shaders/flat_color.frag"
#define FLAT_COLOR_SHADER "FlatColor"

#define TEXTURE_VERT   "examples/assets/shaders/texture.vert"
#define TEXTURE_FRAG   "examples/assets/shaders/texture.frag"
#define TEXTURE_SHADER "TextureShader"

#define TEXTURE_SQUARE_ARROW "examples/assets/textures/square_arrow.png"

#define ATTR_POSITION "a_Position"
#define ATTR_TEXTURE  "a_TexCoord"

#define UNIFORM_COLOR         "u_Color"
#define UNIFORM_TILING_FACTOR "u_TilingFactor"
#define UNIFORM_TEXTURE       "u_Texture"

#define TEXTURE_SLOT 0

#define TILING_FACTOR_DEF 1.0f
#define SQR_TYPE_DEF      SquareLayer::COLORED
#define SQR_COLOR_DEF     glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)

namespace GE::Examples {

SquareLayer::SquareLayer(bool show_gui_demo)
    : GuiLayer{show_gui_demo, "Square Layer"}
{}

void SquareLayer::onAttach()
{
    GE_PROFILE_FUNC();

    // clang-format off
    std::array<float, VERT_PER_QUAD * VERT_ELEM_COUNT> vertices{
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
    };
    // clang-format on

    Shared<VertexBuffer> vbo =
        VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
    BufferLayout layout{{GE_ELEMENT_FLOAT3, ATTR_POSITION},
                        {GE_ELEMENT_FLOAT2, ATTR_TEXTURE}};
    vbo->setLayout(layout);

    std::array<uint32_t, IND_PER_QUAD> indexes = {0, 1, 2, 2, 3, 0};
    Shared<IndexBuffer> ibo = IndexBuffer::create(indexes.data(), indexes.size());

    m_vao = VertexArray::create();
    m_vao->addVertexBuffer(vbo);
    m_vao->setIndexBuffer(ibo);

    auto color_shader =
        m_shader_library.load(FLAT_COLOR_VERT, FLAT_COLOR_FRAG, FLAT_COLOR_SHADER);
    GE_ASSERT_MSG(color_shader, "Failed to load '{}' shader", FLAT_COLOR_SHADER);

    auto texture_shader =
        m_shader_library.load(TEXTURE_VERT, TEXTURE_FRAG, TEXTURE_SHADER);
    GE_ASSERT_MSG(texture_shader, "Failed to load '{}' shader", TEXTURE_SHADER);

    m_texture = Texture2D::create(TEXTURE_SQUARE_ARROW);
    texture_shader->bind();
    texture_shader->setUniformInt(UNIFORM_TEXTURE, TEXTURE_SLOT);
    texture_shader->setUniformFloat(UNIFORM_TILING_FACTOR, TILING_FACTOR_DEF);

    m_square_type = SQR_TYPE_DEF;
    m_square_color = SQR_COLOR_DEF;
}

void SquareLayer::onDetach()
{
    GE_PROFILE_FUNC();

    m_shader_library.clear();
    m_vao.reset();
    m_texture.reset();
}

void SquareLayer::onUpdate(Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    {
        GE_PROFILE_SCOPE("SquareLayer Prepare");
        m_camera_controller.onUpdate(delta_time);
        RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});
    }

    {
        GE_PROFILE_SCOPE("SquareLayer Draw");
        Begin<Renderer> begin{m_camera_controller.getCamera()};
        Renderer::submit(bindCurrentShader(), m_vao);
    }
}

void SquareLayer::onEvent(Event* event)
{
    GE_PROFILE_FUNC();

    GuiLayer::onEvent(event);
}

void SquareLayer::onGuiRender()
{
    GE_PROFILE_FUNC();

    GuiLayer::onGuiRender();
    ImGui::Begin("Settings");
    ImGui::RadioButton("Colored square", &m_square_type, SquareType::COLORED);
    ImGui::ColorEdit4("Square color", glm::value_ptr(m_square_color));
    ImGui::RadioButton("Textured square", &m_square_type, SquareType::TEXTURED);
    auto* texture_id = reinterpret_cast<ImTextureID>(m_texture->getNativeID());
    ImGui::Image(texture_id, {128.0, 128.0});
    ImGui::End();
}

Shared<ShaderProgram> SquareLayer::bindCurrentShader()
{
    GE_PROFILE_FUNC();

    switch (m_square_type) {
        case SquareType::COLORED: {
            auto color_shader = m_shader_library.get(FLAT_COLOR_SHADER);
            color_shader->bind();
            color_shader->setUniformFloat4(UNIFORM_COLOR, m_square_color);
            return color_shader;
        }
        case SquareType::TEXTURED: {
            auto texture_shader = m_shader_library.get(TEXTURE_SHADER);
            texture_shader->setUniformFloat4(UNIFORM_COLOR, glm::vec4{1.0f});
            texture_shader->bind();
            m_texture->bind(TEXTURE_SLOT);
            return texture_shader;
        }
        default: GE_ASSERT_MSG(false, "Unknown square type: {}", m_square_type);
    }

    return nullptr;
}

} // namespace GE::Examples
