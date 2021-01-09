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

#include "view_projection_camera.h"

#include "ge/debug/profile.h"

namespace GE {

ViewProjectionCamera::ViewProjectionCamera()
{
    GE_PROFILE_FUNC();

    calculateVPMatrix();
}

void ViewProjectionCamera::setDistance(float distance)
{
    GE_PROFILE_FUNC();

    m_distance = distance;
    calculateVPMatrix();
}

void ViewProjectionCamera::setOrientation(const glm::quat& orientation)
{
    GE_PROFILE_FUNC();

    m_orientation = orientation;
    calculateVPMatrix();
}

void ViewProjectionCamera::setFocalPoint(const glm::vec3& focal_point)
{
    GE_PROFILE_FUNC();

    m_focal_point = focal_point;
    calculateVPMatrix();
}

glm::vec3 ViewProjectionCamera::getUpDirection() const
{
    GE_PROFILE_FUNC();

    return glm::rotate(m_orientation, {0.0f, 1.0f, 0.0f});
}

glm::vec3 ViewProjectionCamera::getRightDirection() const
{
    GE_PROFILE_FUNC();

    return glm::rotate(m_orientation, {1.0f, 0.0f, 0.0f});
}

glm::vec3 ViewProjectionCamera::getForwardDirection() const
{
    GE_PROFILE_FUNC();

    return glm::rotate(m_orientation, {0.0f, 0.0f, -1.0f});
}

void ViewProjectionCamera::calculateVPMatrix()
{
    GE_PROFILE_FUNC();

    m_position = calculatePosition();
    auto transform = glm::translate(glm::mat4{1.0f}, m_position);
    m_view_mat = glm::inverse(transform * glm::toMat4(m_orientation));
    m_vp_mat = getProjectionMatrix() * m_view_mat;
}

glm::vec3 ViewProjectionCamera::calculatePosition()
{
    GE_PROFILE_FUNC();

    return m_focal_point - getForwardDirection() * m_distance;
}

} // namespace GE