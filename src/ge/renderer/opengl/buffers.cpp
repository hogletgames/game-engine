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

#include "buffers.h"
#include "opengl_utils.h"

#include <glad/glad.h>

namespace {

using BufferType = ::GE::OpenGL::BufferBase::Type;

GLenum toGLBufferType(BufferType type)
{
    switch (type) {
        case BufferType::VERTEX: return GL_ARRAY_BUFFER;
        case BufferType::INDEX: return GL_ELEMENT_ARRAY_BUFFER;
        default: break;
    }

    GE_CORE_ASSERT(false, "Unknown OpenGL buffer type: {}", static_cast<int>(type));
    return 0;
}

} // namespace

namespace GE::OpenGL {

BufferBase::BufferBase(Type type, void* data, uint32_t size)
    : m_gl_type{toGLBufferType(type)}
{
    GE_CORE_ASSERT(m_gl_type, "Unknown buffer type");
    GLCall(glCreateBuffers(1, &m_id));
    GLCall(glBindBuffer(m_gl_type, m_id));
    GLCall(glBufferData(m_gl_type, size, data, GL_STATIC_DRAW));
}

BufferBase::~BufferBase()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void BufferBase::bindBuffer() const
{
    GLCall(glBindBuffer(m_gl_type, m_id));
}

void BufferBase::unbindBuffer() const
{
    GLCall(glBindBuffer(m_gl_type, 0));
}

} // namespace GE::OpenGL
