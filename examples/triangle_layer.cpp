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

#define VER_COUNT            3
#define VERTEX_SOURCE_FILE   "examples/shaders/pass_through.vert"
#define FRAGMENT_SOURCE_FILE "examples/shaders/pass_through.frag"

namespace GE::Examples {

TriangleLayer::TriangleLayer(bool show_gui_demo)
    : GuiLayer{show_gui_demo, "Triangle Layer"}
    , m_camera{-1.6f, 1.6f, -0.9f, 0.9f} // NOLINT
{
    GE_PROFILE_FUNC();

    initializeTriangle();
}

void TriangleLayer::onUpdate()
{
    GE_PROFILE_FUNC();

    {
        GE_PROFILE_SCOPE("TriangleLayer Prepare");
        RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});
    }

    {
        GE_PROFILE_SCOPE("TriangleLayer Draw");
        Begin<Renderer> begin{m_camera};
        Renderer::submit(m_triangle_shader, m_triangle_vao);
    }
}

void TriangleLayer::initializeTriangle()
{
    GE_PROFILE_FUNC();

    // NOLINTNEXTLINE
    std::array<float, VER_COUNT* 7> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // NOLINT
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // NOLINT
        0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // NOLINT
    };

    Shared<VertexBuffer> vertex_buffer =
        VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
    BufferLayout layout{{GE_ELEMENT_FLOAT3, "a_Position"},
                        {GE_ELEMENT_FLOAT4, "a_Color"}};
    vertex_buffer->setLayout(layout);

    std::array<uint32_t, VER_COUNT> indices = {0, 1, 2};
    Shared<IndexBuffer> index_buffer =
        IndexBuffer::create(indices.data(), indices.size());

    m_triangle_vao = VertexArray::create();
    m_triangle_vao->addVertexBuffer(vertex_buffer);
    m_triangle_vao->setIndexBuffer(index_buffer);

    Shared<Shader> vertex_shader = Shader::create(GE_VERTEX_SHADER);
    vertex_shader->compileFromFile(VERTEX_SOURCE_FILE);

    Shared<Shader> fragment_shader = Shader::create(GE_FRAGMENT_SHADER);
    fragment_shader->compileFromFile(FRAGMENT_SOURCE_FILE);

    m_triangle_shader = ShaderProgram::create();
    m_triangle_shader->addShaders({vertex_shader, fragment_shader});
    m_triangle_shader->link();
}

} // namespace GE::Examples
