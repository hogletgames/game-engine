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

#include "ge/debug/profile.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace {

constexpr auto TEXTURE_SQUARE_ARROW = "examples/assets/textures/square_arrow.png";
constexpr auto TEXTURE_BLUE_SQRS = "examples/assets/textures/blue_squares.png";

constexpr float ZOOM_X10{10.0f};
constexpr float ZOOM_X0_75{0.75f};

constexpr float ROTATION_90D_PER_1S{90.0f};

} // namespace

namespace GE::Examples {

Renderer2DLayer::Renderer2DLayer(bool show_gui_demo)
    : GuiLayer{show_gui_demo, "Renderer2D Layer"}
{
    GE_PROFILE_FUNC();
}

void Renderer2DLayer::onAttach()
{
    GE_PROFILE_FUNC();

    m_editable_quad.color = {0.2f, 0.3f, 0.8f, 1.0f};
    m_editable_quad.pos = {-1.5f, 0.0f};

    m_red_quad.color = {0.8f, 0.3f, 0.3f, 1.0f};
    m_red_quad.size *= ZOOM_X0_75;

    m_tex_blue_sqrs_quad.texture = Texture2D::create(TEXTURE_BLUE_SQRS);
    m_tex_blue_sqrs_quad.pos = {1.5f, 1.5f};

    m_tex_arrow_quad.texture = Texture2D::create(TEXTURE_SQUARE_ARROW);
    m_tex_arrow_quad.size *= ZOOM_X10;
    m_tex_arrow_quad.tiling_factor *= ZOOM_X10;
    m_tex_arrow_quad.rotation = 45.0f;
}
void Renderer2DLayer::onDetach()
{
    GE_PROFILE_FUNC();

    m_tex_blue_sqrs_quad.texture.reset();
    m_tex_arrow_quad.texture.reset();
}

void Renderer2DLayer::onUpdate(Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    {
        GE_PROFILE_SCOPE("Renderer2DLayer Prepare");
        m_camera_controller.onUpdate(delta_time);
        RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});

        m_tex_blue_sqrs_quad.rotation +=
            static_cast<float>(delta_time.sec()) * ROTATION_90D_PER_1S;
    }

    {
        GE_PROFILE_SCOPE("Renderer2DLayer Draw");

        Begin<Renderer2D> begin{m_camera_controller.getCamera()};
        Renderer2D::draw(m_editable_quad);
        Renderer2D::draw(m_red_quad);
        Renderer2D::draw(m_tex_blue_sqrs_quad);
        Renderer2D::draw(m_tex_arrow_quad);
    }
}

void Renderer2DLayer::onGuiRender()
{
    GE_PROFILE_FUNC();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_editable_quad.color));
    ImGui::End();
}

} // namespace GE::Examples
