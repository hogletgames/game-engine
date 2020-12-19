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

#include "framebuffer.h"
#include "opengl_utils.h"

#include "ge/core/log.h"
#include "ge/debug/profile.h"

#include <glad/glad.h>

namespace GE::OpenGL {

Framebuffer::Framebuffer(const GE::Framebuffer::properties_t& props)
    : m_props{props}
{
    GE_PROFILE_FUNC();

    create();
}

Framebuffer::~Framebuffer()
{
    GE_PROFILE_FUNC();

    release();
}

void Framebuffer::bind()
{
    GE_PROFILE_FUNC();

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    GLCall(glViewport(0, 0, m_props.width, m_props.height));
}

void Framebuffer::unbind()
{
    GE_PROFILE_FUNC();

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::resize(const glm::vec2& size)
{
    GE_PROFILE_FUNC();

    if (size.x <= 0 || size.y <= 0) {
        GE_CORE_ERR("Attempted to resize framebuffer to ({}, {})", size.x, size.y);
        return;
    }

    m_props.width = size.x;
    m_props.height = size.y;

    create();
}

void Framebuffer::create()
{
    GE_PROFILE_FUNC();

    if (m_id != GL_NONE) {
        release();
    }

    // Color attachment
    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_color_attachment_id));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_props.width, m_props.height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Depth attachment
    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_depth_attachment_id));
    GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_props.width,
                          m_props.height));

    // Frame buffer
    GLCall(glCreateFramebuffers(1, &m_id));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                  m_color_attachment_id, 0));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_TEXTURE_2D, m_depth_attachment_id, 0));

    GLenum framebuffer_status{GL_NONE};
    GLCall(framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    GE_CORE_ASSERT_MSG(framebuffer_status == GL_FRAMEBUFFER_COMPLETE,
                       "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::release()
{
    GE_PROFILE_FUNC();

    GLCall(glDeleteFramebuffers(1, &m_id));
    GLCall(glDeleteTextures(1, &m_depth_attachment_id));
    GLCall(glDeleteTextures(1, &m_color_attachment_id));
}

} // namespace GE::OpenGL
