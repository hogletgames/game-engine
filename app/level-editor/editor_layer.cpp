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
#include "editor_scene_renderer.h"
#include "editor_state.h"
#include "panels/editor_camera_panel.h"
#include "panels/properties_panel.h"
#include "panels/scene_hierarchy_panel.h"
#include "panels/statistic_panel.h"
#include "panels/viewport_panel.h"

#include "ge/debug/profile.h"
#include "ge/ge.h"

#include <ImGuizmo.h>

using WindowProps = GE::Window::properties_t;

namespace LE {

EditorLayer::EditorLayer()
{
    GE_PROFILE_FUNC();
}

void EditorLayer::onAttach()
{
    GE_PROFILE_FUNC();

    GE::Framebuffer::properties_t framebuffer_props{};
    framebuffer_props.width = GE::Application::getWindow().getWidth();
    framebuffer_props.height = GE::Application::getWindow().getHeight();
    GE::Shared<GE::Framebuffer> framebuffer = GE::Framebuffer::create(framebuffer_props);

    auto scene = GE::makeShared<GE::Scene>();

    auto square = scene->createEntity("Square");
    square.addComponent<GE::SpriteRendererComponent>(GE::Color::GREEN);
    auto& square_rotation = square.getComponent<GE::TransformComponent>().rotation;
    square_rotation = glm::radians(glm::vec3{45.0f, 45.0f, 0.0f});

    auto main_camera = scene->createCamera("Main Camera");
    main_camera.addComponent<GE::NativeScriptComponent>()
        .bind<GE::CameraControllerScript>(main_camera);
    scene->setMainCamera(main_camera);

    auto camera = GE::makeShared<GE::ViewProjectionCamera>();
    auto camera_controller = GE::makeShared<GE::VPCameraController>(camera);
    m_scene_renderer = GE::makeShared<EditorSceneRenderer>(camera_controller, scene);

    m_editor_state = GE::makeShared<EditorState>();
    m_editor_state->setFramebuffer(framebuffer);
    m_editor_state->setScene(scene);
    m_editor_state->setCameraController(camera_controller);

    m_panels = {GE::makeShared<ViewportPanel>(m_editor_state),
                GE::makeShared<SceneHierarchyPanel>(m_editor_state),
                GE::makeShared<PropertiesPanel>(m_editor_state),
                GE::makeShared<StatisticPanel>(),
                GE::makeShared<EditorCameraPanel>(camera)};
}

void EditorLayer::onDetach()
{
    GE_PROFILE_FUNC();

    m_scene_renderer.reset();
    m_panels.clear();
    m_editor_state.reset();
}

void EditorLayer::onUpdate(GE::Timestamp dt)
{
    GE_PROFILE_FUNC();

    updateViewport();

    if (m_editor_state->isVPFocused()) {
        m_editor_state->cameraController()->onUpdate(dt);
    }

    auto& framebuffer = m_editor_state->framebuffer();
    GE::Renderer2D::resetStats();

    framebuffer->bind();

    GE::RenderCommand::clear(GE::Color::DARK_GREY);
    m_scene_renderer->onUpdate(dt);

    framebuffer->unbind();
}

void EditorLayer::onEvent(GE::Event* event)
{
    GE_PROFILE_FUNC();

    m_editor_state->cameraController()->onEvent(event);

    GE::EventDispatcher dispatcher{event};
    dispatcher.dispatch<GE::KeyPressedEvent>(GE_BIND_EVENT_FN(onKeyPressed));
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

bool EditorLayer::onKeyPressed(const GE::KeyPressedEvent& event)
{
    // Gizmo
    switch (event.getKeyCode()) {
        case GE::KeyCode::Q: m_editor_state->setGizmoType(GIZMO_TYPE_UNKNOWN); break;
        case GE::KeyCode::W: m_editor_state->setGizmoType(ImGuizmo::TRANSLATE); break;
        case GE::KeyCode::E: m_editor_state->setGizmoType(ImGuizmo::ROTATE); break;
        case GE::KeyCode::R: m_editor_state->setGizmoType(ImGuizmo::SCALE); break;
        default: break;
    }

    return false;
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
        m_editor_state->cameraController()->resize(viewport);
        m_editor_state->scene()->onViewportResize(viewport);
    }
}

} // namespace LE
