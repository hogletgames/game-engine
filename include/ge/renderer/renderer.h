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

#ifndef GE_RENDERER_RENDERER_H_
#define GE_RENDERER_RENDERER_H_

#include <ge/core/core.h>
#include <ge/renderer/render_command.h>

#include <glm/glm.hpp>

namespace GE {

namespace Uniforms {

constexpr auto VP_MATRIX = "u_ViewProjection";
constexpr auto TRANSFORM = "u_Transform";
constexpr auto COLOR = "u_Color";
constexpr auto TEXTURES = "u_Textures";

} // namespace Uniforms

namespace Attributes {

constexpr auto POS = "a_Position";
constexpr auto COLOR = "a_Color";
constexpr auto TEX_COORD = "a_TexCoord";
constexpr auto TEX_INDEX = "a_TexIndex";
constexpr auto TILING_FACTOR = "a_TilingFactor";

} // namespace Attributes

namespace Paths {

constexpr auto ASSETS_DIR = "assets";
constexpr auto COLOR_SHADER = "shaders/flat_color";
constexpr auto TEXTURE_SHADER = "shaders/texture";

} // namespace Paths

class OrthographicCamera;
class ShaderProgram;
class WindowResizedEvent;

class GE_API Renderer
{
public:
    static bool initialize(RendererAPI::API api);
    static void shutdown();

    static bool onWindowResized(const WindowResizedEvent& event);

    static void begin(const OrthographicCamera& camera);
    static void end();

    static void submit(const Shared<ShaderProgram>& shader,
                       const Shared<VertexArray>& vertex_array,
                       const glm::mat4& transform = glm::mat4{1.0});

    static RendererAPI::API getAPI() { return RenderCommand::getAPI(); }
    static const RendererAPI::capabilities_t& getCapabilities();

private:
    Renderer() = default;

    static Renderer* get()
    {
        static Renderer instance;
        return &instance;
    }

    glm::mat4 m_vp_matrix{1.0f};
};

} // namespace GE

#endif // GE_RENDERER_RENDERER_H_
