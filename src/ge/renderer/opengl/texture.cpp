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

#include "texture.h"
#include "opengl_utils.h"

#include "ge/debug/profile.h"

#include <glad/glad.h>
#include <stb_image.h>

#define DESIRED_CHANNELS_NONE 0

namespace {

std::pair<GLenum, GLenum> toGLFormats(int channels)
{
    switch (channels) {
        case 3: return {GL_RGB8, GL_RGB};
        case 4: return {GL_RGBA8, GL_RGBA};
        default: break;
    }

    GE_CORE_ASSERT_MSG(false, "Unsupported channels number: {}", channels);
    return {0, 0};
}

} // namespace

namespace GE::OpenGL {

Texture2D::Texture2D(std::string path)
    : m_path{std::move(path)}
{
    GE_PROFILE_FUNC();

    int width{};
    int height{};
    int channels{};
    stbi_uc* data{nullptr};

    {
        GE_PROFILE_SCOPE("OpenGL::Texture2D Load Texture");
        stbi_set_flip_vertically_on_load(1);
        data =
            stbi_load(m_path.c_str(), &width, &height, &channels, DESIRED_CHANNELS_NONE);
        GE_CORE_ASSERT_MSG(data, "Failed to load texture '{}'", path);
    }

    m_width = width;
    m_height = height;
    m_bpp = channels;

    auto [internal_format, data_format] = toGLFormats(m_bpp);
    createTexture(internal_format);
    GLCall(glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, data_format,
                               GL_UNSIGNED_BYTE, data));
    stbi_image_free(data);
}

Texture2D::Texture2D(uint32_t width, uint32_t height, uint32_t bpp)
    : m_width{width}
    , m_height{height}
    , m_bpp(bpp)
{
    GE_PROFILE_FUNC();

    auto [internal_format, data_format] = toGLFormats(m_bpp);
    createTexture(internal_format);
}

Texture2D::~Texture2D()
{
    GE_PROFILE_FUNC();

    GLCall(glDeleteTextures(1, &m_id));
}

void Texture2D::setData(const void* data, uint32_t size)
{
    size_t expected_size = m_width * m_height * m_bpp;
    GE_CORE_ASSERT_MSG(size == expected_size, "Wrong texture size: {} != {}", size,
                       expected_size);
    auto [internal_format, data_format] = toGLFormats(m_bpp);
    GLCall(glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, data_format,
                               GL_UNSIGNED_BYTE, data));
}

void Texture2D::bind(uint32_t slot) const
{
    GE_PROFILE_FUNC();

    GLCall(glBindTextureUnit(slot, m_id));
}

void Texture2D::createTexture(uint32_t internal_format)
{
    GE_PROFILE_FUNC();

    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_id));
    GLCall(glTextureStorage2D(m_id, 1, internal_format, m_width, m_height));

    GLCall(glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

} // namespace GE::OpenGL
