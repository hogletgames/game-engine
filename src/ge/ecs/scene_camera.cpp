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

#include "scene_camera.h"

#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GE {

SceneCamera::SceneCamera()
{
    GE_PROFILE_FUNC();

    calculateProjection();
}

void SceneCamera::setProjectionType(ProjectionType projection_type)
{
    GE_PROFILE_FUNC();

    m_projection_type = projection_type;
    calculateProjection();
}

void SceneCamera::setViewport(const glm::vec2& viewport)
{
    GE_PROFILE_FUNC();

    m_aspect_ratio = std::abs(viewport.x / viewport.y);
    calculateProjection();
}

void SceneCamera::setPerspective(const SceneCamera::perspective_params_t& perspective)
{
    GE_PROFILE_FUNC();

    m_projection_type = ProjectionType::PERSPECTIVE;
    m_perspective = perspective;
    calculateProjection();
}

void SceneCamera::setOrthographic(const SceneCamera::orthographic_params_t& orthographic)
{
    GE_PROFILE_FUNC();

    m_projection_type = ProjectionType::ORTHOGRAPHIC;
    m_orthographic = orthographic;
    calculateProjection();
}

void SceneCamera::calculateProjection()
{
    GE_PROFILE_FUNC();

    switch (m_projection_type) {
        case ProjectionType::PERSPECTIVE: calculatePerspectiveProjection(); break;
        case ProjectionType::ORTHOGRAPHIC: calculateOrthographicProjection(); break;
        default:
            GE_CORE_ERR("Unknown projection type: {}", toString(m_projection_type));
            break;
    }
}

void SceneCamera::calculatePerspectiveProjection()
{
    GE_PROFILE_FUNC();

    m_projection = glm::perspective(glm::radians(m_perspective.fov), m_aspect_ratio,
                                    m_perspective.near, m_perspective.far);
}

void SceneCamera::calculateOrthographicProjection()
{
    GE_PROFILE_FUNC();

    float left = -m_orthographic.size * m_aspect_ratio * 0.5f;
    float right = m_orthographic.size * m_aspect_ratio * 0.5f;
    float bottom = -m_orthographic.size * 0.5f;
    float top = m_orthographic.size * 0.5f;

    m_projection =
        glm::ortho(left, right, bottom, top, m_orthographic.near, m_orthographic.far);
}

std::string toString(SceneCamera::ProjectionType projection)
{
    using Projection = SceneCamera::ProjectionType;
    static const std::unordered_map<Projection, std::string> projection_to_string{
        {Projection::PERSPECTIVE, "Perspective"},
        {Projection::ORTHOGRAPHIC, "Orthographic"}};

    return toType(projection_to_string, projection, std::string("Unknown"));
}

} // namespace GE
