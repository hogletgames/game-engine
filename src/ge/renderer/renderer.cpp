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

#include "renderer.h"
#include "render_command.h"
#include "shader_program.h"
#include "vertex_array.h"
#include "view_projection_camera.h"

#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"
#include "ge/window/window_event.h"

namespace GE {

bool Renderer::initialize(RendererAPI::API api)
{
    GE_PROFILE_FUNC();
    GE_CORE_DBG("Initialize Renderer");

    if (!RenderCommand::initialize(api)) {
        GE_CORE_ERR("Failed to initialize Renderer");
        return false;
    }

    return true;
}

void Renderer::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Shutdown Renderer");
    RenderCommand::shutdown();
}

bool Renderer::onWindowResized(const WindowResizedEvent& event)
{
    GE_PROFILE_FUNC();

    RenderCommand::setViewport(0, 0, event.getWidth(), event.getHeight());
    return false;
}

void Renderer::begin(const ViewProjectionCamera& camera)
{
    GE_PROFILE_FUNC();

    get()->m_vp_matrix = camera.getVPMatrix();
}

void Renderer::end() {}

void Renderer::submit(const Shared<ShaderProgram>& shader,
                      const Shared<VertexArray>& vertex_array, const glm::mat4& transform)
{
    GE_PROFILE_FUNC();

    shader->bind();
    shader->setUniformMat4(Uniforms::VP_MATRIX, get()->m_vp_matrix);
    shader->setUniformMat4(Uniforms::TRANSFORM, transform);

    vertex_array->bind();
    RenderCommand::draw(vertex_array);
}

const RendererAPI::capabilities_t& Renderer::getCapabilities()
{
    return RenderCommand::getCapabilities();
}

} // namespace GE
