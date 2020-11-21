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

#include <map>

namespace GE {

class Entity;
class OrthographicCamera;
class Texture2D;
class VertexArray;
class VertexBuffer;

class GE_API Renderer2D
{
public:
    struct quad_t {
        glm::vec2 pos{POS_DEFAULT};
        glm::vec2 size{SIZE_DEFAULT};
        glm::vec4 color{COLOR_DEFAULT};
        Shared<Texture2D> texture;
        float depth{DEPTH_DEFAULT};
        float tiling_factor{TILING_FACT_DEFAULT};
        float rotation{ROTATION_DEFAULT};

        static constexpr glm::vec2 POS_DEFAULT{0.0f, 0.0f};
        static constexpr glm::vec2 SIZE_DEFAULT{1.0f, 1.0f};
        static constexpr glm::vec4 COLOR_DEFAULT{1.0f};
        static constexpr float DEPTH_DEFAULT{0.0f};
        static constexpr float TILING_FACT_DEFAULT{1.0f};
        static constexpr float ROTATION_DEFAULT{0.0f};
    };

    struct statistics_t {
        uint32_t draw_calls_count{};
        uint32_t quad_count{};
        uint32_t vertex_count{};
        uint32_t index_count{};
    };

    ~Renderer2D();

    static bool initialize(const std::string& assets_dir);
    static void shutdown();

    static const std::string& getAssetsDir();

    static void begin(const OrthographicCamera& camera);
    static void end();

    static void draw(const Entity& entity);
    static void draw(const quad_t& quad);
    static void flush();

    static const statistics_t& getStats();
    static void resetStats();

private:
    struct draw_object_t {
        glm::mat4 transform{1.0f};
        glm::vec4 color{1.0f};
        Shared<Texture2D> texture;
        float tiling_factor{1.0f};
    };

    struct quad_vertex_t {
        glm::vec3 pos{};
        glm::vec4 color{};
        glm::vec2 tex_coord{};
        float tex_index{};
        float tiling_factor{};
    };

    static Renderer2D* get()
    {
        static Renderer2D instance;
        return &instance;
    }

    using QuadVertexArray = std::vector<quad_vertex_t>;
    using QVAIterator = QuadVertexArray::iterator;

    Renderer2D();

    void draw(const draw_object_t& draw_object);

    void initializeTextures();

    Shared<ShaderProgram> loadShader(const std::string& name,
                                     const std::string& shader_dir);

    uint32_t getTexSlot(const Shared<Texture2D>& texture);
    void resetBatch();

    std::string m_assets_dir;
    Shared<VertexArray> m_quad_vao;
    Shared<VertexBuffer> m_quad_vbo;
    ShaderLibrary m_shader_library;

    uint32_t m_index_count{};
    QuadVertexArray m_quad_vert_array;
    QVAIterator m_curr_vert_element;
    std::map<uint32_t, Shared<Texture2D>> m_textures;
    uint32_t m_curr_free_tex_slot{};

    statistics_t m_stats{};
};

} // namespace GE

#endif // GE_RENDERER_RENDERER_2D_H_
