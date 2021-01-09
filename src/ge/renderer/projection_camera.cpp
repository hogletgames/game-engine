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

#include "projection_camera.h"

#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"

#include <glm/gtc/matrix_transform.hpp>

namespace {

const auto PROJECTION_UNKNOWN_STR = "Unknown";
const auto PROJECTION_PERSPECTIVE_STR = "Perspective";
const auto PROJECTION_ORTHOGRAPHIC_STR = "Orthographic";

} // namespace

namespace GE {

ProjectionCamera::ProjectionCamera()
{
    GE_PROFILE_FUNC();

    calculateProjection();
}

void ProjectionCamera::setViewport(const glm::vec2& viewport)
{
    GE_PROFILE_FUNC();

    m_viewport = viewport;
    calculateProjection();
}

void ProjectionCamera::setProjectionType(ProjectionType projection_type)
{
    GE_PROFILE_FUNC();

    m_projection_type = projection_type;
    calculateProjection();
}

void ProjectionCamera::setPerspectiveParams(const perspective_params_t& perspective)
{
    GE_PROFILE_FUNC();

    m_perspective = perspective;
    calculateProjection();
}

void ProjectionCamera::setOrthographicParams(const orthographic_params_t& orthographic)
{
    GE_PROFILE_FUNC();

    m_orthographic = orthographic;
    calculateProjection();
}

void ProjectionCamera::calculateProjection()
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

void ProjectionCamera::calculatePerspectiveProjection()
{
    GE_PROFILE_FUNC();

    m_proj_mat = glm::perspective(glm::radians(m_perspective.fov), getAspectRatio(),
                                  m_perspective.near, m_perspective.far);
}

void ProjectionCamera::calculateOrthographicProjection()
{
    GE_PROFILE_FUNC();

    float aspect_ratio = getAspectRatio();
    float left = -m_orthographic.size * aspect_ratio * 0.5f;
    float right = m_orthographic.size * aspect_ratio * 0.5f;
    float bottom = -m_orthographic.size * 0.5f;
    float top = m_orthographic.size * 0.5f;

    m_proj_mat =
        glm::ortho(left, right, bottom, top, m_orthographic.near, m_orthographic.far);
}

std::string toString(ProjectionCamera::ProjectionType projection)
{
    using Projection = ProjectionCamera::ProjectionType;
    static const std::unordered_map<Projection, std::string> projection_to_string = {
        {Projection::UNKNOWN, PROJECTION_UNKNOWN_STR},
        {Projection::PERSPECTIVE, PROJECTION_PERSPECTIVE_STR},
        {Projection::ORTHOGRAPHIC, PROJECTION_ORTHOGRAPHIC_STR}};

    return toType(projection_to_string, projection, std::string(PROJECTION_UNKNOWN_STR));
}

ProjectionCamera::ProjectionType toCameraProjection(const std::string& projection)
{
    using Projection = ProjectionCamera::ProjectionType;
    static const std::unordered_map<std::string, Projection> string_to_projection = {
        {PROJECTION_UNKNOWN_STR, Projection::UNKNOWN},
        {PROJECTION_PERSPECTIVE_STR, Projection::PERSPECTIVE},
        {PROJECTION_ORTHOGRAPHIC_STR, Projection::ORTHOGRAPHIC}};

    return toType(string_to_projection, projection, Projection::UNKNOWN);
}

} // namespace GE
