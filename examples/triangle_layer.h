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

// NOLINTNEXTLINE
#ifndef GE_EXAMPLES_TRIANGLE_LAYER_H_
#define GE_EXAMPLES_TRIANGLE_LAYER_H_

#include "gui_layer.h"

#include <ge/ge.h>

#include <imgui.h>

#define VER_COUNT            3
#define VERTEX_SOURCE_FILE   "examples/shaders/pass_through.vert"
#define FRAGMENT_SOURCE_FILE "examples/shaders/pass_through.frag"

namespace GE::Examples {

class GE_API TriangleLayer: public GuiLayer
{
public:
    explicit TriangleLayer(bool show_gui_demo)
        : GuiLayer{show_gui_demo, "Triangle Layer"}
    {
        // NOLINTNEXTLINE
        std::array<float, VER_COUNT* 7> vertices = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // NOLINT
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // NOLINT
            0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // NOLINT
        };

        m_vertex_buffer =
            VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
        m_layout = BufferLayout{{GE_ELEMENT_FLOAT3, "a_Position"},
                                {GE_ELEMENT_FLOAT4, "a_Color"}};
        m_vertex_buffer->setLayout(m_layout);

        std::array<uint32_t, VER_COUNT> indices = {0, 1, 2};
        m_index_buffer = IndexBuffer::create(indices.data(), indices.size());

        m_vertex_array = VertexArray::create();
        m_vertex_array->addVertexBuffer(m_vertex_buffer);
        m_vertex_array->setIndexBuffer(m_index_buffer);

        m_vertex_shader = Shader::create(GE_VERTEX_SHADER);
        m_vertex_shader->compileFromFile(VERTEX_SOURCE_FILE);

        m_fragment_shader = Shader::create(GE_FRAGMENT_SHADER);
        m_fragment_shader->compileFromFile(FRAGMENT_SOURCE_FILE);

        m_shader_program = ShaderProgram::create();
        m_shader_program->addShaders({m_vertex_shader, m_fragment_shader});
        m_shader_program->link();
    }

    void onUpdate() override
    {
        RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});
        Begin<Renderer> begin{};
        m_shader_program->bind();
        Renderer::submit(m_vertex_array);
    }

private:
    Shared<VertexBuffer> m_vertex_buffer;
    Shared<IndexBuffer> m_index_buffer;
    Shared<VertexArray> m_vertex_array;
    BufferLayout m_layout;
    Shared<Shader> m_vertex_shader;
    Shared<Shader> m_fragment_shader;
    Scoped<ShaderProgram> m_shader_program;
};

} // namespace GE::Examples

#endif // GE_EXAMPLES_TRIANGLE_LAYER_H_
