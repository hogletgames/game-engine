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

#ifndef GE_RENDERER_VIEW_PROJECTION_CAMERA_H_
#define GE_RENDERER_VIEW_PROJECTION_CAMERA_H_

#include <ge/renderer/projection_camera.h>

#include <glm/gtx/quaternion.hpp>

namespace GE {

class ViewProjectionCamera: public ProjectionCamera
{
public:
    ViewProjectionCamera();

    const glm::mat4& getViewMatrix() const { return m_view_mat; }
    const glm::mat4& getVPMatrix() const { return m_vp_mat; }

    void setDistance(float distance);
    float getDistance() const { return m_distance; }

    void setOrientation(const glm::quat& orientation);
    glm::quat getOrientation() const { return m_orientation; }

    void setFocalPoint(const glm::vec3& focal_point);
    const glm::vec3& getFocalPoint() const { return m_focal_point; }
    const glm::vec3& getPosition() const { return m_position; }

    glm::vec3 getUpDirection() const;
    glm::vec3 getRightDirection() const;
    glm::vec3 getForwardDirection() const;

    void calculateVPMatrix();

private:
    glm::vec3 calculatePosition();

    glm::mat4 m_view_mat{1.0f};
    glm::mat4 m_vp_mat{1.0f};

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_focal_point{0.0f, 0.0f, 0.0f};
    glm::quat m_orientation{0.0f, 0.0f, 0.0f, 0.0f};
    float m_distance{10.0f};
};

} // namespace GE

#endif // GE_RENDERER_VIEW_PROJECTION_CAMERA_H_
