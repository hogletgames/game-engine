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

#include "renderer_api.h"
#include "opengl_utils.h"

#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"

#include <glad/glad.h>

namespace {

void dumpCapabilities([[maybe_unused]] const GE::RendererAPI::capabilities_t& caps)
{
    GE_CORE_INFO("Renderer ({}) capabilities:", GE_OPEN_GL_API);
    GE_CORE_INFO("Texture slots max: {}", caps.max_texture_slots);
}

GE::RendererAPI::capabilities_t loadCapabilities()
{
    GLint max_textures{};

    GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures));

    GE::RendererAPI::capabilities_t caps;
    caps.max_texture_slots = max_textures;

    dumpCapabilities(caps);

    return caps;
}

} // namespace

namespace GE::OpenGL {

void RendererAPI::clear(const glm::vec4& color)
{
    GE_PROFILE_FUNC();

    GLCall(glClearColor(color.r, color.g, color.b, color.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RendererAPI::draw(const Shared<VertexArray>& vertex_array)
{
    GE_PROFILE_FUNC();

    GLsizei index_count = vertex_array->getIndexBuffer()->getCount();
    draw(index_count);
}

void RendererAPI::draw(uint32_t index_count)
{
    GE_PROFILE_FUNC();

    GLCall(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr));
}

void RendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    GE_PROFILE_FUNC();

    GLCall(glViewport(x, y, width, height));
}

const RendererAPI::capabilities_t& RendererAPI::getCapabilities()
{
    if (m_capabilities == nullptr) {
        m_capabilities = makeScoped<capabilities_t>(loadCapabilities());
    }

    return *m_capabilities;
}

} // namespace GE::OpenGL
