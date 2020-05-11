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

#include "vertex_array.h"
#include "opengl_utils.h"

#include "ge/debug/profile.h"

#include <glad/glad.h>

namespace {

GLenum toGlBaseType(::GE::BufferElement::Type type)
{
    switch (type) {
        case GE_ELEMENT_FLOAT:
        case GE_ELEMENT_FLOAT2:
        case GE_ELEMENT_FLOAT3:
        case GE_ELEMENT_FLOAT4:
        case GE_ELEMENT_MAT3:
        case GE_ELEMENT_MAT4: return GL_FLOAT;
        case GE_ELEMENT_INT:
        case GE_ELEMENT_INT2:
        case GE_ELEMENT_INT3:
        case GE_ELEMENT_INT4: return GL_INT;
        case GE_ELEMENT_BOOL: return GL_BOOL;
        default: break;
    }

    GE_CORE_ASSERT(false, "Unknown element type: {}", static_cast<int>(type));
    return 0;
}

} // namespace

namespace GE::OpenGL {

VertexArray::VertexArray()
{
    GE_PROFILE_FUNC();

    GLCall(glCreateVertexArrays(1, &m_id));
}

VertexArray::~VertexArray()
{
    GE_PROFILE_FUNC();

    GLCall(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::bind() const
{
    GE_PROFILE_FUNC();

    GLCall(glBindVertexArray(m_id));
}

void VertexArray::unbind() const
{
    GE_PROFILE_FUNC();

    GLCall(glBindVertexArray(0));
}

void VertexArray::addVertexBuffer(Shared<VertexBuffer> vertex_buffer)
{
    GE_PROFILE_FUNC();

    const auto& layout = vertex_buffer->getLayout();
    GE_CORE_ASSERT(!layout.getElements().empty(), "Vertex Buffer has no layout");

    GLCall(glBindVertexArray(m_id));
    vertex_buffer->bind();

    for (const auto& element : layout) {
        GLint size = element.getComponentCount();
        GLsizei type = toGlBaseType(element.type());
        GLboolean normalized = element.normalized() ? GL_TRUE : GL_FALSE;
        GLsizei stride = layout.getStride();
        const auto* offset = reinterpret_cast<const GLvoid*>(element.offset());

        GLCall(glEnableVertexAttribArray(m_vb_idx));
        GLCall(glVertexAttribPointer(m_vb_idx, size, type, normalized, stride, offset));
        m_vb_idx++;
    }

    m_vertices.emplace_back(std::move(vertex_buffer));
}

void VertexArray::setIndexBuffer(Shared<IndexBuffer> index_buffer)
{
    GE_PROFILE_FUNC();

    GLCall(glBindVertexArray(m_id));
    index_buffer->bind();
    m_index_buffer = std::move(index_buffer);
}

} // namespace GE::OpenGL
