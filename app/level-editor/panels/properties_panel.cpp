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

#include "properties_panel.h"
#include "editor_state.h"

#include "ge/core/type_list.h"
#include "ge/debug/profile.h"
#include "ge/ecs/camera_controller_script.h"
#include "ge/ecs/components.h"
#include "ge/ecs/entity.h"
#include "ge/renderer/colors.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

using Components =
    GE::TypeList<GE::TagComponent, GE::TransformComponent, GE::CameraComponent,
                 GE::SpriteRendererComponent, GE::NativeScriptComponent>;

namespace {
/**
 * Tag Component
 */
void drawComponent(GE::TagComponent* component, LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    static constexpr size_t tag_buffer_size{256};
    static std::array<char, tag_buffer_size> tag_buffer{};
    static GE::Entity::ID previous_entity_id{GE::Entity::nullID()};
    GE::Entity::ID current_entity_id = editor_state->selectedEntity().getID();

    if (previous_entity_id != current_entity_id) {
        size_t buffer_end = std::min(tag_buffer.size() - 1, component->tag.size());
        component->tag.copy(tag_buffer.data(), tag_buffer.size());
        tag_buffer[buffer_end] = '\0';

        previous_entity_id = current_entity_id;
    }

    ImGui::InputText("Tag", tag_buffer.data(), tag_buffer.size());
    ImGui::SameLine();

    if (ImGui::Button("Apply")) {
        component->tag = tag_buffer.data();
    }
}

/**
 * Transform
 */
void drawComponent(GE::TransformComponent* component,
                   [[maybe_unused]] LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    glm::vec3 rotation = glm::degrees(component->rotation);

    ImGui::DragFloat3("Translation", glm::value_ptr(component->translation), 0.1f);

    if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation))) {
        component->rotation = glm::radians(rotation);
    }

    ImGui::DragFloat3("Scale", glm::value_ptr(component->scale), 0.1f);

    if (ImGui::Button("Reset Transform")) {
        component->translation = {0.0f, 0.0f, 0.0f};
        component->rotation = {0.0f, 0.0f, 0.0f};
        component->scale = {1.0f, 1.0f, 1.0f};
    }
}

/**
 * Camera Component
 */
void drawCameraProjectionCombo(GE::CameraComponent* component)
{
    GE_PROFILE_FUNC();

    using Projection = GE::SceneCamera::ProjectionType;

    auto current_projection = component->camera.getProjectionType();
    static constexpr std::array<Projection, 2> all_projections{Projection::PERSPECTIVE,
                                                               Projection::ORTHOGRAPHIC};

    if (ImGui::BeginCombo("Projection", GE::toString(current_projection).c_str())) {
        for (auto projection : all_projections) {
            bool is_selected = current_projection == projection;

            if (ImGui::Selectable(GE::toString(projection).c_str(), is_selected)) {
                current_projection = projection;
                component->camera.setProjectionType(projection);
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
}

void drawCameraProjectionPerspective(GE::CameraComponent* component)
{
    GE_PROFILE_FUNC();

    auto& camera = component->camera;
    auto params = camera.getPerspective();

    bool is_projection_changed = ImGui::DragFloat("Vertical FOV", &params.fov);
    is_projection_changed |= ImGui::DragFloat("Near", &params.near, 0.1f);
    is_projection_changed |= ImGui::DragFloat("Far", &params.far, 10.0f);

    if (is_projection_changed) {
        camera.setPerspective(params);
    }

    if (ImGui::Button("Reset Projection")) {
        camera.setPerspective({});
    }
}

void drawCameraProjectionOrthographic(GE::CameraComponent* component,
                                      LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    auto& camera = component->camera;
    auto params = camera.getOrthographic();

    bool is_projection_changed = ImGui::DragFloat("Size", &params.size, 0.25);
    is_projection_changed |= ImGui::DragFloat("Near", &params.near, 0.25);
    is_projection_changed |= ImGui::DragFloat("Far", &params.far, 0.25);

    if (is_projection_changed) {
        camera.setOrthographic(params);
    }

    if (ImGui::Button("Reset Projection")) {
        camera.setOrthographic({});
    }

    if (ImGui::Checkbox("Fixed aspect ratio", &component->fixed_aspect_ratio) &&
        !component->fixed_aspect_ratio) {
        component->camera.setViewport(editor_state->viewport());
    }
}

void drawComponent(GE::CameraComponent* component, LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    using Projection = GE::SceneCamera::ProjectionType;

    auto& scene = editor_state->scene();
    auto current_projection = component->camera.getProjectionType();
    bool is_primary = scene->getMainCamera() == editor_state->selectedEntity();

    if (ImGui::Checkbox("Primary Camera", &is_primary)) {
        const auto& camera_entity = is_primary ? editor_state->selectedEntity()
                                               : GE::Entity{};
        scene->setMainCamera(camera_entity);
    }

    drawCameraProjectionCombo(component);

    if (current_projection == Projection::PERSPECTIVE) {
        drawCameraProjectionPerspective(component);
    } else if (current_projection == Projection::ORTHOGRAPHIC) {
        drawCameraProjectionOrthographic(component, editor_state);
    }
}

/**
 * Sprite Renderer
 */
void drawComponent(GE::SpriteRendererComponent* component,
                   [[maybe_unused]] LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    ImGui::ColorEdit4("Color", glm::value_ptr(component->color));
}

/**
 * Native Script
 */
void drawNativeScript(GE::NativeScriptComponent* component)
{
    GE_PROFILE_FUNC();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", component->scriptName().c_str());
    ImGui::SameLine();

    if (ImGui::Button("Remove Script")) {
        component->destroy();
    }
}

void drawNativeScriptAdd(GE::NativeScriptComponent* component,
                         LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    if (ImGui::Button("Add Script")) {
        ImGui::OpenPopup("Add Script");
    }

    if (ImGui::BeginPopup("Add Script")) {
        if (ImGui::MenuItem("Camera Controller")) {
            auto& entity = editor_state->selectedEntity();
            component->bind<GE::CameraControllerScript>(entity);
        }

        ImGui::EndPopup();
    }
}

void drawComponent(GE::NativeScriptComponent* component, LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    if (component->isScriptBound()) {
        drawNativeScript(component);
    } else {
        drawNativeScriptAdd(component, editor_state);
    }
}

/**
 * Component
 */
void drawAddNewComponent(GE::Entity* entity)
{
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("Add Component");
    }

    if (ImGui::BeginPopup("Add Component")) {
        GE::forEachType<Components>([entity](auto component) {
            using Component = decltype(component);

            if (entity->hasComponent<Component>()) {
                return;
            }

            if (ImGui::MenuItem(Component::name().c_str())) {
                entity->addComponent<Component>();
            }
        });

        ImGui::EndPopup();
    }
}

template<typename Component>
void draw(LE::EditorState* editor_state)
{
    GE_PROFILE_FUNC();

    auto& entity = editor_state->selectedEntity();

    if (!entity.hasComponent<Component>()) {
        return;
    }

    std::string name = Component::name();
    ImGuiColorEditFlags flags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
        ImGuiTreeNodeFlags_FramePadding;

    bool is_expanded = ImGui::TreeNodeEx(name.c_str(), flags, "%s", name.c_str());
    bool remove_entity{false};

    if (ImGui::BeginPopupContextItem()) {
        if (!GE::isListContains<GE::Scene::DefaultComponents, Component>() &&
            ImGui::MenuItem("Remove")) {
            remove_entity = true;
        }

        ImGui::EndPopup();
    }

    if (is_expanded) {
        drawComponent(&entity.getComponent<Component>(), editor_state);
        ImGui::TreePop();
    }

    if (remove_entity) {
        entity.removeComponent<Component>();
    }
}

} // namespace

namespace LE {

PropertiesPanel::PropertiesPanel(GE::Shared<EditorState> editor_state)
    : m_editor_state{std::move(editor_state)}
{
    GE_PROFILE_FUNC();
}

void PropertiesPanel::onGuiRender()
{
    GE_PROFILE_FUNC();

    if (ImGui::Begin("Properties") && !m_editor_state->selectedEntity().isNull()) {
        GE::forEachType<Components>(
            [this](auto component) { draw<decltype(component)>(m_editor_state.get()); });
        ImGui::Separator();
        drawAddNewComponent(&m_editor_state->selectedEntity());
    }

    ImGui::End();
}

void PropertiesPanel::clear()
{
    GE_PROFILE_FUNC();

    m_editor_state.reset();
}

} // namespace LE
