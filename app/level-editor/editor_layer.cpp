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
#include "panels/statistic_panel.h"

#include "ge/debug/profile.h"
#include "ge/ge.h"

#include <imgui.h>

namespace {

constexpr float ASPECT_RATION_DEFAULT{static_cast<float>(WINDOW_WIDTH_DEF) /
                                      WINDOW_HEIGHT_DEF};

} // namespace

namespace LE {

EditorLayer::EditorLayer()
    : m_camera_controller{ASPECT_RATION_DEFAULT, true}
{
    GE_PROFILE_FUNC();
}

void EditorLayer::onAttach()
{
    GE_PROFILE_FUNC();

    m_editable_quad.color = {0.8f, 0.3f, 0.3f, 1.0f};

    m_panels = {GE::makeShared<StatisticPanel>()};
}

void EditorLayer::onDetach()
{
    GE_PROFILE_FUNC();

    m_panels.clear();
}

void EditorLayer::onUpdate(GE::Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    m_camera_controller.onUpdate(delta_time);

    GE::RenderCommand::clear({1.0f, 0.0f, 1.0f, 1.0f});

    {
        GE::Begin<GE::Renderer2D> begin{m_camera_controller.getCamera()};
        GE::Renderer2D::resetStats();
        GE::Renderer2D::draw(m_editable_quad);
    }
}

void EditorLayer::onEvent(GE::Event* event)
{
    GE_PROFILE_FUNC();

    m_camera_controller.onEvent(event);
}

void EditorLayer::onGuiRender()
{
    GE_PROFILE_FUNC();

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
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

} // namespace LE
