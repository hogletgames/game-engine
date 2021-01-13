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
#include "viewport_panel.h"
#include "editor_state.h"

#include "ge/debug/profile.h"
#include "ge/gui/gui.h"
#include "ge/math/math.h"
#include "ge/renderer/framebuffer.h"
#include "ge/window/input.h"

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace LE {

ViewportPanel::ViewportPanel(GE::Shared<EditorState> editor_state)
    : m_editor_state{std::move(editor_state)}
{
    GE_PROFILE_FUNC();
}

void ViewportPanel::onGuiRender()
{
    GE_PROFILE_FUNC();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

    if (ImGui::Begin("Viewport")) {
        bool is_vp_focused = ImGui::IsWindowFocused();
        bool is_vp_inactive = !is_vp_focused || !ImGui::IsWindowHovered();
        GE::Gui::blockEvents(is_vp_inactive);

        ImVec2 vp_panel_size = ImGui::GetContentRegionAvail();
        auto framebuffer_tex = m_editor_state->framebuffer()->getColorAttachmentID();
        auto* fb_texture = reinterpret_cast<ImTextureID>(framebuffer_tex);

        ImGui::Image(fb_texture, vp_panel_size, {0, 1}, {1, 0});
        drawGizmo(&m_editor_state->selectedEntity());

        m_editor_state->setViewport({vp_panel_size.x, vp_panel_size.y});
        m_editor_state->setIsVPFocused(is_vp_focused);
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void ViewportPanel::drawGizmo(GE::Entity* entity)
{
    const auto& camera_entity = m_editor_state->scene()->getMainCamera();

    if (entity->isNull() || camera_entity.isNull() ||
        m_editor_state->gizmoType() == GIZMO_TYPE_UNKNOWN) {
        return;
    }

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    auto window_pos = ImGui::GetWindowPos();
    auto window_size = ImGui::GetWindowSize();
    ImGuizmo::SetRect(window_pos.x, window_pos.y, window_size.x, window_size.y);

    const auto& camera = m_editor_state->cameraController()->getCamera();
    const auto& camera_projection = camera->getProjectionMatrix();
    const auto& camera_view = camera->getViewMatrix();

    // TODO: use it in runtime
    // const auto& camera = camera_entity.getComponent<GE::CameraComponent>().camera;
    // const auto& camera_projection = camera.getProjectionMatrix();
    // const auto& camera_transform = camera_entity.getComponent<GE::TransformComponent>();
    // auto camera_view = glm::inverse(camera_transform.getTransform());

    auto& tc = entity->getComponent<GE::TransformComponent>();
    auto transform = tc.getTransform();

    std::array<float, 3> snap_values{};
    auto type = static_cast<ImGuizmo::OPERATION>(m_editor_state->gizmoType());
    float* snap = GE::Input::isKeyPressed(GE::KeyCode::LCTRL) ? snap_values.data()
                                                              : nullptr;

    if (snap != nullptr) {
        float snap_value = type == ImGuizmo::ROTATE ? 45.0f : 0.5f;
        std::fill(snap_values.begin(), snap_values.end(), snap_value);
    }

    ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection),
                         type, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
                         snap_values.data());

    if (ImGuizmo::IsUsing()) {
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;

        if (GE::Math::decomposeTransform(transform, &translation, &rotation, &scale)) {
            glm::vec3 rotation_delta = rotation - tc.rotation;

            tc.translation = translation;
            tc.rotation += rotation_delta;
            tc.scale = scale;
        }
    }
}

} // namespace LE
