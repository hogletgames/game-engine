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

#include "renderer_2d.h"
#include "buffers.h"
#include "orthographic_camera.h"
#include "render_command.h"
#include "renderer.h"
#include "shader_program.h"
#include "texture.h"
#include "vertex_array.h"

#include "ge/core/asserts.h"
#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"
#include "ge/ecs/components.h"
#include "ge/ecs/entity.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <filesystem>
#include <numeric>

namespace {

constexpr size_t VERT_PER_QUAD{4};
constexpr size_t IND_PER_QUAD{6};

constexpr size_t DRAW_CALL_QUAD_MAX{20000};
constexpr size_t DRAW_CALL_VERT_MAX{DRAW_CALL_QUAD_MAX * VERT_PER_QUAD};
constexpr size_t DRAW_CALL_IND_MAX{DRAW_CALL_QUAD_MAX * IND_PER_QUAD};

constexpr uint32_t WHITE_TEX_IDX{0};

constexpr auto TEXTURE_SHADER = "TextureShader";

std::pair<std::string, std::string> getShadersPath(const std::string& assets_dir,
                                                   const std::string& shader_dir)
{
    GE_PROFILE_FUNC();

    using std::filesystem::path;
    std::string shader_path = path(assets_dir).append(shader_dir);
    return {shader_path + GE_VERT_EXT, shader_path + GE_FRAG_EXT};
}

glm::mat4 getTransformMat(const GE::Renderer2D::quad_t& quad)
{
    using Quad = GE::Renderer2D::quad_t;
    glm::mat4 transform{1.0};

    if (quad.pos != Quad::POS_DEFAULT || quad.depth != Quad::DEPTH_DEFAULT) {
        transform = glm::translate(glm::mat4{1.0f}, {quad.pos, quad.depth});
    }

    if (quad.rotation != Quad::ROTATION_DEFAULT) {
        transform =
            glm::rotate(transform, glm::radians(quad.rotation), {0.0f, 0.0f, 1.0f});
    }

    if (quad.size != Quad::SIZE_DEFAULT) {
        transform = glm::scale(transform, {quad.size, 1.0});
    }

    return transform;
}

} // namespace

namespace GE {

Renderer2D::~Renderer2D() = default;

bool Renderer2D::initialize(const std::string& assets_dir)
{
    GE_PROFILE_FUNC();

    get()->m_assets_dir = assets_dir;

    GE_CORE_DBG("Initialize Renderer 2D");
    GE_CORE_INFO("Renderer 2D: Assets dir: '{}'", get()->m_assets_dir);

    if (get()->loadShader(TEXTURE_SHADER, Paths::TEXTURE_SHADER) == nullptr) {
        GE_CORE_ERR("Failed to load '{}'", Paths::TEXTURE_SHADER);
        return false;
    }

    auto& quad_vbo = get()->m_quad_vbo;
    quad_vbo = VertexBuffer::create(DRAW_CALL_VERT_MAX * sizeof(quad_vertex_t));
    quad_vbo->setLayout({{GE_ELEMENT_FLOAT3, Attributes::POS},
                         {GE_ELEMENT_FLOAT4, Attributes::COLOR},
                         {GE_ELEMENT_FLOAT2, Attributes::TEX_COORD},
                         {GE_ELEMENT_FLOAT, Attributes::TEX_INDEX},
                         {GE_ELEMENT_FLOAT, Attributes::TILING_FACTOR}});

    std::vector<uint32_t> indices(DRAW_CALL_IND_MAX);
    constexpr std::array<uint32_t, IND_PER_QUAD> one_quad_indices = {0, 1, 2, 2, 3, 0};

    for (size_t i{0}; i < DRAW_CALL_QUAD_MAX; i++) {
        auto indices_begin = std::next(indices.begin(), i * IND_PER_QUAD);
        std::transform(one_quad_indices.begin(), one_quad_indices.end(), indices_begin,
                       [i](uint32_t index) { return index + (i * VERT_PER_QUAD); });
    }

    Shared<IndexBuffer> ibo = IndexBuffer::create(indices.data(), indices.size());

    auto& quad_vao = get()->m_quad_vao;
    quad_vao = VertexArray::create();
    quad_vao->addVertexBuffer(quad_vbo);
    quad_vao->setIndexBuffer(ibo);

    get()->initializeTextures();
    get()->resetBatch();

    return true;
}

void Renderer2D::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Shutdown Renderer 2D");
    get()->m_assets_dir.clear();
    get()->m_quad_vao.reset();
    get()->m_quad_vbo.reset();
    get()->m_shader_library.clear();

    get()->resetBatch();
    get()->m_textures.clear();
}

const std::string& Renderer2D::getAssetsDir()
{
    return get()->m_assets_dir;
}

void Renderer2D::begin(const OrthographicCamera& camera)
{
    GE_PROFILE_FUNC();

    get()->begin(camera.getVPMatrix());
}

void Renderer2D::begin(const Entity& camera)
{
    GE_PROFILE_FUNC();

    const auto& transform = camera.getComponent<TransformComponent>().getTransform();
    const auto& scene_camera = camera.getComponent<CameraComponent>().camera;

    get()->begin(scene_camera.getProjection() * glm::inverse(transform));
}

void Renderer2D::end()
{
    GE_PROFILE_FUNC();

    flush();
}

void Renderer2D::draw(const Entity& entity)
{
    GE_PROFILE_FUNC();

    draw_object_t draw_object{};
    draw_object.transform = entity.getComponent<TransformComponent>().getTransform();
    draw_object.color = entity.getComponent<SpriteRendererComponent>().color;

    get()->draw(draw_object);
}

void Renderer2D::draw(const quad_t& quad)
{
    GE_PROFILE_FUNC();

    get()->draw(draw_object_t{getTransformMat(quad), quad.color, quad.texture,
                              quad.tiling_factor});
}

void Renderer2D::flush()
{
    GE_PROFILE_FUNC();

    auto& curr_vert_element = get()->m_curr_vert_element;
    auto& vert_array = get()->m_quad_vert_array;

    if (curr_vert_element == vert_array.begin()) {
        return;
    }

    uint32_t vert_count = std::distance(vert_array.begin(), curr_vert_element);
    get()->m_quad_vbo->setData(vert_array.data(), vert_count * sizeof(quad_vertex_t));

    for (uint32_t i{0}; i < get()->m_curr_free_tex_slot; i++) {
        get()->m_textures[i]->bind(i);
    }

    get()->m_quad_vao->bind();
    RenderCommand::draw(get()->m_index_count);

    get()->m_stats.draw_calls_count++;
    get()->resetBatch();
}

const Renderer2D::statistics_t& Renderer2D::getStats()
{
    auto& stats = get()->m_stats;
    stats.vertex_count = stats.quad_count * VERT_PER_QUAD;
    stats.index_count = stats.quad_count * IND_PER_QUAD;
    return stats;
}

void Renderer2D::resetStats()
{
    GE_PROFILE_FUNC();

    get()->m_stats = {};
}

Renderer2D::Renderer2D()
    : m_quad_vert_array(DRAW_CALL_VERT_MAX)
{}

void Renderer2D::begin(const glm::mat4& vp_matrix)
{
    GE_PROFILE_FUNC();

    auto tex_shader = m_shader_library.get(TEXTURE_SHADER);
    tex_shader->bind();
    tex_shader->setUniformMat4(Uniforms::VP_MATRIX, vp_matrix);
}

void Renderer2D::draw(const draw_object_t& draw_object)
{
    GE_PROFILE_FUNC();

    static constexpr glm::mat4 quad_ver_pos = {{-0.5f, -0.5f, 0.0f, 1.0f},
                                               {0.5f, -0.5f, 0.0f, 1.0f},
                                               {0.5f, 0.5f, 0.0f, 1.0f},
                                               {-0.5f, 0.5f, 0.0f, 1.0f}};
    static constexpr glm::mat4x2 tex_coords = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    auto curr_elem_distance = std::distance(m_curr_vert_element, m_quad_vert_array.end());
    size_t vert_elements_left = curr_elem_distance > 0 ? curr_elem_distance : 0;

    if (vert_elements_left < VERT_PER_QUAD) {
        flush();
    }

    uint32_t tex_slot = getTexSlot(draw_object.texture);

    for (size_t i{0}; i < VERT_PER_QUAD; i++) {
        m_curr_vert_element->pos = draw_object.transform * quad_ver_pos[i];
        m_curr_vert_element->color = draw_object.color;
        m_curr_vert_element->tex_coord = tex_coords[i];
        m_curr_vert_element->tex_index = tex_slot;
        m_curr_vert_element->tiling_factor = draw_object.tiling_factor;
        ++m_curr_vert_element;
    }

    m_index_count += IND_PER_QUAD;
    m_stats.quad_count++;
}

void Renderer2D::initializeTextures()
{
    GE_PROFILE_FUNC();

    constexpr uint32_t white_tex_data{0xFFFFFFFF};
    const uint32_t max_tex_slots{RenderCommand::getCapabilities().max_texture_slots};

    auto white_texture = Texture2D::create(1, 1, 4);
    white_texture->setData(&white_tex_data, sizeof(white_tex_data));

    for (uint32_t i{0}; i < max_tex_slots; i++) {
        m_textures.emplace(i, nullptr);
    }

    m_textures[WHITE_TEX_IDX] = std::move(white_texture);

    std::vector<int> samplers(max_tex_slots);
    std::iota(samplers.begin(), samplers.end(), 0);
    auto tex_shader = get()->m_shader_library.get(TEXTURE_SHADER);
    tex_shader->bind();
    tex_shader->setUniformIntArray(Uniforms::TEXTURES, samplers.data(), samplers.size());
}

Shared<ShaderProgram> Renderer2D::loadShader(const std::string& name,
                                             const std::string& shader_dir)
{
    GE_PROFILE_FUNC();

    auto [vert, frag] = getShadersPath(get()->m_assets_dir, shader_dir);
    return m_shader_library.load(vert, frag, name);
}

uint32_t Renderer2D::getTexSlot(const Shared<Texture2D>& texture)
{
    GE_PROFILE_FUNC();

    if (texture == nullptr) {
        return WHITE_TEX_IDX;
    }

    if (m_curr_free_tex_slot >= Renderer::getCapabilities().max_texture_slots) {
        flush();
    }

    auto tex_slot_finder = [&texture](const auto& pair) {
        if (pair.second == nullptr) {
            return false;
        }

        return *pair.second == *texture;
    };

    auto it = std::find_if(m_textures.cbegin(), m_textures.cend(), tex_slot_finder);

    if (it != m_textures.cend()) {
        return it->first;
    }

    uint32_t curr_slot = m_curr_free_tex_slot;
    m_textures[curr_slot] = texture;
    m_curr_free_tex_slot++;

    return curr_slot;
}

void Renderer2D::resetBatch()
{
    GE_PROFILE_FUNC();

    m_curr_vert_element = get()->m_quad_vert_array.begin();
    m_index_count = 0;
    m_curr_free_tex_slot = WHITE_TEX_IDX + 1;
}

} // namespace GE
