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

#include "render_command.h"

#include "ge/core/log.h"

namespace GE {

bool RenderCommand::initialize(RendererAPI::API api)
{
    GE_CORE_DBG("Initialize Renderer Command");

    auto& renderer_api = get()->m_renderer_api;

    if (renderer_api = RendererAPI::create(api); renderer_api == nullptr) {
        GE_CORE_ERR("Failed to create RenderAPI");
        return false;
    }

    return true;
}

void RenderCommand::shutdown()
{
    GE_CORE_DBG("Shutdown Renderer Command");
    get()->m_renderer_api.reset();
}

void RenderCommand::clear(const glm::vec4& color)
{
    get()->m_renderer_api->clear(color);
}

void RenderCommand::draw(const Shared<VertexArray>& vertex_array)
{
    get()->m_renderer_api->draw(vertex_array);
}

void RenderCommand::draw(uint32_t index_count)
{
    get()->m_renderer_api->draw(index_count);
}

void RenderCommand::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    get()->m_renderer_api->setViewport(x, y, width, height);
}

RendererAPI::API RenderCommand::getAPI()
{
    return get()->m_renderer_api->getAPI();
}

const RendererAPI::capabilities_t& RenderCommand::getCapabilities()
{
    return get()->m_renderer_api->getCapabilities();
}

} // namespace GE
