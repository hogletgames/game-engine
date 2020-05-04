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

#ifndef GE_RENDERER_RENDERER_2D_H_
#define GE_RENDERER_RENDERER_2D_H_

#include <ge/core/core.h>
#include <ge/renderer/shader_program.h>

#include <glm/glm.hpp>

#define GE_QUAD_POS_DEF       glm::vec2(0.0f, 0.0f)
#define GE_QUAD_SIZE_DEF      glm::vec2(1.0f, 1.0f)
#define GE_QUAD_DEPTH_DEF     0.0f
#define GE_QUAD_TILE_FACT_DEF 1.0f
#define GE_QUAD_ROT_DEF       0.0f

namespace GE {

class OrthographicCamera;
class Texture2D;
class VertexArray;

class GE_API Renderer2D
{
public:
    struct quad_params_t {
        glm::vec2 pos{};
        glm::vec2 size{};
        float depth{};
        float tiling_factor{};
        float rotation{};

        // NOLINTNEXTLINE
        quad_params_t(const glm::vec2& pos = GE_QUAD_POS_DEF,
                      const glm::vec2& size = GE_QUAD_SIZE_DEF,
                      float depth = GE_QUAD_DEPTH_DEF,
                      float tiling_factor = GE_QUAD_TILE_FACT_DEF,
                      float rotation = GE_QUAD_ROT_DEF)
            : pos{pos}
            , size{size}
            , depth{depth}
            , tiling_factor{tiling_factor}
            , rotation{rotation}
        {}
    };

    ~Renderer2D();

    static bool initialize(const std::string& assets_dir);
    static void shutdown();

    static const std::string& getAssetsDir();

    static void begin(const OrthographicCamera& camera);
    static void end();

    static void drawQuad(const glm::vec4& color,
                         const quad_params_t& params = quad_params_t{});
    static void drawQuad(const Shared<Texture2D>& texture,
                         const quad_params_t& params = quad_params_t{});
    static void drawQuad(const Shared<Texture2D>& texture, const glm::vec4& color,
                         const quad_params_t& params = quad_params_t{});

private:
    static Renderer2D* get()
    {
        static Renderer2D instance;
        return &instance;
    }

    Shared<ShaderProgram> loadShader(const std::string& name,
                                     const std::string& shader_dir);

    std::string m_assets_dir;
    Shared<VertexArray> m_quad_vao;
    ShaderLibrary m_shader_library;
    Shared<Texture2D> m_white_texture;
};

} // namespace GE

#endif // GE_RENDERER_RENDERER_2D_H_
