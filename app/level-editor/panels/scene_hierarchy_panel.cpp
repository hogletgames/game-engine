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

#include "scene_hierarchy_panel.h"
#include "editor_state.h"

#include "ge/debug/profile.h"
#include "ge/ecs/components.h"
#include "ge/ecs/entity.h"
#include "ge/window/input.h"

#include <imgui.h>

namespace LE {

SceneHierarchyPanel::SceneHierarchyPanel(GE::Shared<EditorState> editor_state)
    : m_editor_state{std::move(editor_state)}
{
    GE_PROFILE_FUNC();
}

void SceneHierarchyPanel::onGuiRender()
{
    GE_PROFILE_FUNC();

    if (ImGui::Begin("Scene Hierarchy")) {
        auto& scene = m_editor_state->scene();
        scene->eachEntity([this](GE::Entity entity) { drawEntityNode(entity); });
    }

    if (GE::Input::isMouseButtonPressed(GE::MouseButton::BUTTON_LEFT) &&
        ImGui::IsWindowHovered()) {
        m_editor_state->setSelectedEntity(GE::Entity{});
    }

    if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right, false)) {
        if (ImGui::MenuItem("Create Entity")) {
            m_editor_state->scene()->createEntity();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void SceneHierarchyPanel::clear()
{
    GE_PROFILE_FUNC();

    m_editor_state.reset();
}

void SceneHierarchyPanel::drawEntityNode(const GE::Entity& entity)
{
    GE_PROFILE_FUNC();

    const auto& tag = entity.getComponent<GE::TagComponent>().tag;
    const void* tree_id = reinterpret_cast<const void*>(entity.getID());

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (entity == m_editor_state->selectedEntity()) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool is_expanded = ImGui::TreeNodeEx(tree_id, flags, "%s", tag.c_str());
    bool remove_entity{false};

    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Remove")) {
            remove_entity = true;
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked()) {
        m_editor_state->setSelectedEntity(entity);
    }

    if (is_expanded) {
        ImGui::TreePop();
    }

    if (remove_entity) {
        m_editor_state->scene()->destroyEntity(entity);

        if (m_editor_state->selectedEntity() == entity) {
            m_editor_state->setSelectedEntity(GE::Entity{});
        }
    }
}

} // namespace LE
