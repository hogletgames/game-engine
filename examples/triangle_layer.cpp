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

#include "triangle_layer.h"

#include "ge/debug/profile.h"

#define VER_COUNT       3
#define VERT_DATA_COUNT 7

#define VERT_PATH   "examples/shaders/pass_through.vert"
#define FRAG_PATH   "examples/shaders/pass_through.frag"
#define SHADER_NAME "PassThrough"

#define ATTR_POSITION "a_Position"
#define ATTR_COLOR    "a_Color"

namespace GE::Examples {

TriangleLayer::TriangleLayer(bool show_gui_demo)
    : GuiLayer{show_gui_demo, "Triangle Layer"}
{}

void TriangleLayer::onAttach()
{
    GE_PROFILE_FUNC();

    // NOLINTNEXTLINE
    std::array<float, VER_COUNT* VERT_DATA_COUNT> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // NOLINT
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // NOLINT
        0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // NOLINT
    };

    uint32_t vert_data_size = vertices.size() * sizeof(float);
    auto vertex_buffer = VertexBuffer::create(vertices.data(), vert_data_size);

    BufferLayout layout{{GE_ELEMENT_FLOAT3, ATTR_POSITION},
                        {GE_ELEMENT_FLOAT4, ATTR_COLOR}};
    vertex_buffer->setLayout(layout);

    std::array<uint32_t, VER_COUNT> indices = {0, 1, 2};
    auto index_buffer = IndexBuffer::create(indices.data(), indices.size());

    m_vao = VertexArray::create();
    m_vao->addVertexBuffer(std::move(vertex_buffer));
    m_vao->setIndexBuffer(std::move(index_buffer));

    auto shader = m_shader_library.load(VERT_PATH, FRAG_PATH, SHADER_NAME);
    GE_ASSERT_MSG(shader, "Failed to load '{}' shader", SHADER_NAME);
}


void TriangleLayer::onDetach()
{
    GE_PROFILE_FUNC();

    m_vao.reset();
    m_shader_library.clear();
}

void TriangleLayer::onUpdate(Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    {
        GE_PROFILE_SCOPE("TriangleLayer Prepare");
        m_camera_controller.onUpdate(delta_time);
        RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});
    }

    {
        GE_PROFILE_SCOPE("TriangleLayer Draw");
        Begin<Renderer> begin{m_camera_controller.getCamera()};
        Renderer::submit(m_shader_library.get(SHADER_NAME), m_vao);
    }
}

void TriangleLayer::onEvent(Event* event)
{
    GE_PROFILE_FUNC();

    GuiLayer::onEvent(event);
}

} // namespace GE::Examples
