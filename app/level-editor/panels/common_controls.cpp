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

#include "common_controls.h"

#include "ge/debug/profile.h"
#include "ge/renderer/view_projection_camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <string>

namespace {

/**
 * Camera
 */
void drawProjectionCameraCombo(GE::ProjectionCamera *camera)
{
    GE_PROFILE_FUNC();

    using Projection = GE::ProjectionCamera::ProjectionType;

    auto current_projection = camera->getProjectionType();
    static constexpr std::array<Projection, 2> all_projections{Projection::PERSPECTIVE,
                                                               Projection::ORTHOGRAPHIC};

    if (ImGui::BeginCombo("Projection", GE::toString(current_projection).c_str())) {
        for (auto projection : all_projections) {
            bool is_selected = current_projection == projection;

            if (ImGui::Selectable(GE::toString(projection).c_str(), is_selected)) {
                current_projection = projection;
                camera->setProjectionType(projection);
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
}

void drawProjectionCameraPerspective(GE::ProjectionCamera *camera)
{
    GE_PROFILE_FUNC();

    auto params = camera->getPerspective();

    bool is_projection_changed = ImGui::DragFloat("Vertical FOV", &params.fov);
    is_projection_changed |= ImGui::DragFloat("Near", &params.near, 0.1f);
    is_projection_changed |= ImGui::DragFloat("Far", &params.far, 10.0f);

    if (is_projection_changed) {
        camera->setPerspectiveParams(params);
    }

    if (ImGui::Button("Reset Projection")) {
        camera->setPerspectiveParams({});
    }
}

void drawProjectionCameraOrthographic(GE::ProjectionCamera *camera)
{
    GE_PROFILE_FUNC();

    auto params = camera->getOrthographic();

    bool is_projection_changed = ImGui::DragFloat("Size", &params.size, 0.25);
    is_projection_changed |= ImGui::DragFloat("Near", &params.near, 0.25);
    is_projection_changed |= ImGui::DragFloat("Far", &params.far, 0.25);

    if (is_projection_changed) {
        camera->setOrthographicParams(params);
    }

    if (ImGui::Button("Reset Projection")) {
        camera->setOrthographicParams({});
    }
}

} // namespace

namespace LE {

void drawProjectionCamera(GE::ProjectionCamera *camera)
{
    GE_PROFILE_FUNC();

    using Projection = GE::ProjectionCamera::ProjectionType;
    auto projection_type = camera->getProjectionType();

    drawProjectionCameraCombo(camera);

    if (projection_type == Projection::PERSPECTIVE) {
        drawProjectionCameraPerspective(camera);
    } else if (projection_type == Projection::ORTHOGRAPHIC) {
        drawProjectionCameraOrthographic(camera);
    }
}

void drawVPCamera(GE::ViewProjectionCamera *camera)
{
    GE_PROFILE_FUNC();

    drawProjectionCamera(camera);
    ImGui::Separator();

    float distance = camera->getDistance();
    auto rotation = glm::degrees(glm::eulerAngles(camera->getOrientation()));
    auto focal_point = camera->getFocalPoint();

    if (ImGui::DragFloat("Distance", &distance)) {
        camera->setDistance(distance);
    }

    if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation))) {
        camera->setOrientation(glm::radians(rotation));
    }

    if (ImGui::DragFloat3("Focal Point", glm::value_ptr(focal_point), 0.1f)) {
        camera->setFocalPoint(focal_point);
    }
}

} // namespace LE
