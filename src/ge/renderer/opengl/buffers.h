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
#ifndef GE_RENDERER_OPENGL_BUFFERS_H_
#define GE_RENDERER_OPENGL_BUFFERS_H_

#include "ge/renderer/buffers.h"

namespace GE::OpenGL {

class BufferBase: public NonCopyable
{
public:
    enum class Type : uint8_t
    {
        NONE = 0,
        VERTEX,
        INDEX
    };

    enum class Usage : uint8_t
    {
        STREAM = 0,
        STATIC,
        DYNAMIC
    };

    BufferBase(Type type, void* data, uint32_t size, Usage usage);
    ~BufferBase() override;

    void bindBuffer() const;
    void unbindBuffer() const;

private:
    uint32_t m_gl_type{0};
    uint32_t m_id{0};
};

class VertexBuffer: public ::GE::VertexBuffer, public BufferBase
{
public:
    VertexBuffer(float* vertices, uint32_t size, Usage usage = Usage::STATIC)
        : BufferBase{Type::VERTEX, vertices, size, usage}
    {}

    void bind() const override { bindBuffer(); }
    void unbind() const override { unbindBuffer(); }

    void setLayout(const BufferLayout& layout) override { m_layout = layout; }
    const BufferLayout& getLayout() const override { return m_layout; }

private:
    BufferLayout m_layout;
};

class IndexBuffer: public ::GE::IndexBuffer, public BufferBase
{
public:
    IndexBuffer(uint32_t* indexes, uint32_t count)
        : BufferBase(Type::INDEX, indexes, sizeof(*indexes) * count, Usage::STATIC)
        , m_count{count}
    {}

    void bind() const override { bindBuffer(); }
    void unbind() const override { unbindBuffer(); }

    uint32_t getCount() const override { return m_count; }

private:
    uint32_t m_count{};
};

} // namespace GE::OpenGL

#endif // GE_RENDERER_OPENGL_BUFFERS_H_
