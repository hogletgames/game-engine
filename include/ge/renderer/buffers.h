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

#ifndef GE_RENDERER_BUFFERS_H_
#define GE_RENDERER_BUFFERS_H_

#include <ge/core/non_copyable.h>
#include <ge/renderer/buffer_layout.h>

#include <memory>

namespace GE {

class GE_API VertexBuffer: public NonCopyable
{
public:
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setLayout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& getLayout() const = 0;

    virtual void setData(const void* data, uint32_t size) = 0;

    static Scoped<VertexBuffer> create(float* vertices, uint32_t size);
    static Scoped<VertexBuffer> create(uint32_t size);
};

class GE_API IndexBuffer: public NonCopyable
{
public:
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    static Scoped<IndexBuffer> create(uint32_t* indexes, uint32_t count);
};

} // namespace GE

#endif // GE_RENDERER_BUFFERS_H_
