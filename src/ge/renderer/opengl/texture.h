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
#ifndef GE_RENDERER_OPENGL_TEXTURE_H_
#define GE_RENDERER_OPENGL_TEXTURE_H_

#include <ge/renderer/texture.h>

namespace GE::OpenGL {

class Texture2D: public ::GE::Texture2D
{
public:
    explicit Texture2D(std::string path);
    Texture2D(uint32_t width, uint32_t height, uint32_t bpp);
    ~Texture2D() override;

    uint32_t getWidth() const override { return m_width; }
    uint32_t getHeight() const override { return m_height; }

    void setData(const void* data, uint32_t size) override;

    uint32_t getNativeID() const override { return m_id; };

    void bind(uint32_t slot) const override;

private:
    void createTexture(uint32_t internal_format);

    std::string m_path;
    uint32_t m_id{0};
    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_bpp{};
};

} // namespace GE::OpenGL

#endif // GE_RENDERER_OPENGL_TEXTURE_H_
