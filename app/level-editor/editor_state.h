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

// NOLINTNEXTLINE(llvm-header-guard)
#ifndef LE_EDITOR_STATE_H_
#define LE_EDITOR_STATE_H_

#include <ge/ecs/entity.h>
#include <ge/ecs/scene.h>
#include <ge/renderer/framebuffer.h>

#include <glm/glm.hpp>

namespace {

inline constexpr int GIZMO_TYPE_UNKNOWN{-1};

}

namespace LE {

class GE_API EditorState
{
public:
    EditorState(GE::Scoped<GE::Framebuffer> framebuffer, GE::Shared<GE::Scene> scene)
        : m_framebuffer{std::move(framebuffer)}
        , m_scene{std::move(scene)}
    {}

    const GE::Scoped<GE::Framebuffer>& framebuffer() const { return m_framebuffer; }
    GE::Scoped<GE::Framebuffer>& framebuffer() { return m_framebuffer; }

    void setViewport(const glm::vec2& viewport) { m_viewport = viewport; }
    const glm::vec2& viewport() const { return m_viewport; }

    void setIsVPFocused(bool is_vp_focused) { m_is_vp_focused = is_vp_focused; }
    bool isVPFocused() const { return m_is_vp_focused; }

    const GE::Shared<GE::Scene>& scene() const { return m_scene; }
    GE::Shared<GE::Scene>& scene() { return m_scene; }

    void setSelectedEntity(const GE::Entity& entity) { m_selected_entity = entity; }
    const GE::Entity& selectedEntity() const { return m_selected_entity; }
    GE::Entity& selectedEntity() { return m_selected_entity; }

    void setGizmoType(int type) { m_gizmo_type = type; }
    int gizmoType() const { return m_gizmo_type; }

private:
    GE::Scoped<GE::Framebuffer> m_framebuffer;
    glm::vec2 m_viewport{0.0f, 0.0f};
    bool m_is_vp_focused{false};

    GE::Shared<GE::Scene> m_scene;
    GE::Entity m_selected_entity;

    int m_gizmo_type{-1};
};

} // namespace LE

#endif // LE_EDITOR_STATE_H_
