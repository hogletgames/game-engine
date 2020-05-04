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

#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <filesystem>

#define VERT_COUNT      4
#define VERT_ELEM_COUNT 5
#define IND_COUNT       6

#define TEXTURE_SHADER "TextureShader"

#define TILING_FACTOR_DEF 1.0f

namespace {

std::pair<std::string, std::string> getShadersPath(const std::string& assets_dir,
                                                   const std::string& shader_dir)
{
    GE_PROFILE_FUNC();

    using std::filesystem::path;
    std::string shader_path = path(assets_dir).append(shader_dir);
    return {shader_path + GE_VERT_EXT, shader_path + GE_FRAG_EXT};
}

glm::mat4 getTransformMat(const GE::Renderer2D::quad_params_t& params)
{
    glm::mat4 transform{1.0};

    if (params.pos != GE_QUAD_POS_DEF || params.depth != GE_QUAD_DEPTH_DEF) {
        transform = glm::translate(glm::mat4{1.0f}, {params.pos, params.depth});
    }

    if (params.rotation != GE_QUAD_ROT_DEF) {
        transform = glm::rotate(transform, params.rotation, {0.0f, 0.0f, 1.0f});
    }

    if (params.size != GE_QUAD_SIZE_DEF) {
        transform = glm::scale(transform, {params.size, 1.0});
    }

    return transform;
}

} // namespace

namespace GE {

Renderer2D::~Renderer2D() = default;

bool Renderer2D::initialize(const std::string& assets_dir)
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Initialize Renderer 2D");

    get()->m_assets_dir = assets_dir;

    GE_CORE_INFO("Renderer 2D: Assets dir: '{}'", get()->m_assets_dir);

    // clang-format off
    std::array<float, VERT_COUNT * VERT_ELEM_COUNT> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
    };
    // clang-format on

    Shared<VertexBuffer> vbo =
        VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
    BufferLayout layout{{GE_ELEMENT_FLOAT3, GE_ATTR_POSITION},
                        {GE_ELEMENT_FLOAT2, GE_ATTR_TEX_COORD}};
    vbo->setLayout(layout);

    std::array<uint32_t, IND_COUNT> indexes = {0, 1, 2, 2, 3, 0};
    Shared<IndexBuffer> ibo = IndexBuffer::create(indexes.data(), indexes.size());

    auto& quad_vao = get()->m_quad_vao;
    quad_vao = VertexArray::create();
    quad_vao->addVertexBuffer(vbo);
    quad_vao->setIndexBuffer(ibo);

    constexpr uint32_t white_tex_data{0xFFFFFFFF};
    auto& white_texture = get()->m_white_texture;
    white_texture = Texture2D::create(1, 1, 4);
    white_texture->setData(&white_tex_data, sizeof(white_tex_data));

    auto tex_shader = get()->loadShader(TEXTURE_SHADER, GE_TEXTURE_SHADER_PATH);

    if (tex_shader == nullptr) {
        GE_CORE_ERR("Failed to load '{}'", GE_COLOR_SHADER_PATH);
        return false;
    }

    tex_shader->bind();
    tex_shader->setUniformInt(GE_UNIFORM_TEXTURE, 0);

    return true;
}

void Renderer2D::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Shutdown Renderer 2D");
    get()->m_assets_dir.clear();
    get()->m_quad_vao.reset();
    get()->m_shader_library.clear();
    get()->m_white_texture.reset();
}

const std::string& Renderer2D::getAssetsDir()
{
    return get()->m_assets_dir;
}

void Renderer2D::begin(const OrthographicCamera& camera)
{
    GE_PROFILE_FUNC();

    auto tex_shader = get()->m_shader_library.get(TEXTURE_SHADER);
    tex_shader->bind();
    tex_shader->setUniformMat4(GE_UNIFORM_VP_MATRIX, camera.getVPMatrix());
}

void Renderer2D::end() {}

void Renderer2D::drawQuad(const glm::vec4& color, const quad_params_t& params)
{
    GE_PROFILE_FUNC();

    drawQuad(get()->m_white_texture, color, params);
}

void Renderer2D::drawQuad(const Shared<Texture2D>& texture, const quad_params_t& params)
{
    GE_PROFILE_FUNC();

    drawQuad(texture, glm::vec4{1.0f}, params);
}

void Renderer2D::drawQuad(const Shared<Texture2D>& texture, const glm::vec4& color,
                          const quad_params_t& params)
{
    GE_PROFILE_FUNC();

    auto tex_shader = get()->m_shader_library.get(TEXTURE_SHADER);
    tex_shader->setUniformFloat4(GE_UNIFORM_COLOR, color);
    tex_shader->setUniformFloat(GE_UNIFORM_TILING_FACTOR, TILING_FACTOR_DEF);

    glm::mat4 transform = getTransformMat(params);
    tex_shader->setUniformMat4(GE_UNIFORM_TRANSFORM, transform);
    tex_shader->setUniformFloat(GE_UNIFORM_TILING_FACTOR, params.tiling_factor);

    texture->bind();
    get()->m_quad_vao->bind();
    RenderCommand::draw(get()->m_quad_vao);
}

Shared<ShaderProgram> Renderer2D::loadShader(const std::string& name,
                                             const std::string& shader_dir)
{
    GE_PROFILE_FUNC();

    auto [vert, frag] = getShadersPath(get()->m_assets_dir, shader_dir);
    return m_shader_library.load(vert, frag, name);
}

} // namespace GE
