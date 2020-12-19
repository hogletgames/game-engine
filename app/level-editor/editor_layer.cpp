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

#include "editor_layer.h"
#include "editor_state.h"
#include "panels/statistic_panel.h"
#include "panels/viewport_panel.h"

#include "ge/debug/profile.h"
#include "ge/ge.h"

#include <imgui.h>

using WindowProps = GE::Window::properties_t;

namespace {

constexpr float ASPECT_RATION_DEFAULT{static_cast<float>(WindowProps::WIDTH_DEFAULT) /
                                      WindowProps::HEIGHT_DEFAULT};

} // namespace

namespace LE {

EditorLayer::EditorLayer()
    : m_vp_camera{ASPECT_RATION_DEFAULT, true}
{
    GE_PROFILE_FUNC();
}

void EditorLayer::onAttach()
{
    GE_PROFILE_FUNC();

    m_editable_quad.color = {0.8f, 0.3f, 0.3f, 1.0f};

    GE::Framebuffer::properties_t framebuffer_props{};
    framebuffer_props.width = GE::Application::getWindow().getWidth();
    framebuffer_props.height = GE::Application::getWindow().getHeight();
    auto framebuffer = GE::Framebuffer::create(framebuffer_props);

    m_editor_state = GE::makeShared<EditorState>(std::move(framebuffer));

    m_panels = {GE::makeShared<ViewportPanel>(m_editor_state),
                GE::makeShared<StatisticPanel>()};
}

void EditorLayer::onDetach()
{
    GE_PROFILE_FUNC();

    m_panels.clear();
    m_editor_state.reset();
}

void EditorLayer::onUpdate(GE::Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    updateViewport();

    if (m_editor_state->isVPFocused()) {
        m_vp_camera.onUpdate(delta_time);
    }

    m_editor_state->framebuffer()->bind();
    GE::RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});

    {
        GE::Begin<GE::Renderer2D> begin{m_vp_camera.getCamera()};
        GE::Renderer2D::resetStats();
        GE::Renderer2D::draw(m_editable_quad);
    }

    m_editor_state->framebuffer()->unbind();
}

void EditorLayer::onEvent(GE::Event* event)
{
    GE_PROFILE_FUNC();

    m_vp_camera.onEvent(event);
}

void EditorLayer::onGuiRender()
{
    GE_PROFILE_FUNC();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    showMenuBar();

    for (auto& panel : m_panels) {
        panel->onGuiRender();
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void EditorLayer::showMenuBar()
{
    GE_PROFILE_FUNC();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                GE::Application::close();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void EditorLayer::updateViewport()
{
    GE_PROFILE_FUNC();

    const auto& framebuffer_props = m_editor_state->framebuffer()->getProps();
    const auto& viewport = m_editor_state->viewport();
    bool is_vp_positive = viewport.x > 0 && viewport.y > 0;
    bool is_vp_changed = framebuffer_props.width != viewport.x ||
                         framebuffer_props.height != viewport.y;

    if (is_vp_changed && is_vp_positive) {
        m_editor_state->framebuffer()->resize(viewport);
        m_vp_camera.resize(viewport);
    }
}

} // namespace LE
