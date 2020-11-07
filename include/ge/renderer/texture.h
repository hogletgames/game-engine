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

#ifndef GE_RENDERER_TEXTURE_H_
#define GE_RENDERER_TEXTURE_H_

#include <ge/core/interface.h>

#include <string>

namespace GE {

class GE_API Texture: public Interface
{
public:
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void setData(const void* data, uint32_t size) = 0;

    virtual uint32_t getNativeID() const = 0;

    virtual void bind(uint32_t slot = 0) const = 0;
};

class GE_API Texture2D: public Texture
{
public:
    static Scoped<Texture2D> create(std::string path);
    static Scoped<Texture2D> create(uint32_t width, uint32_t height, uint32_t bpp);
};

inline bool operator<(const Texture& lhs, const Texture& rhs)
{
    return lhs.getNativeID() < rhs.getNativeID();
}

inline bool operator==(const Texture& lhs, const Texture& rhs)
{
    return lhs.getNativeID() == rhs.getNativeID();
}

} // namespace GE

#endif // GE_RENDERER_TEXTURE_H_
