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

#ifndef GE_RENDERER_ORTHOGRAPHIC_CAMERA_H_
#define GE_RENDERER_ORTHOGRAPHIC_CAMERA_H_

#include <ge/core/core.h>

#include <glm/glm.hpp>

namespace GE {

class GE_API OrthographicCamera
{
public:
    OrthographicCamera() = default;
    OrthographicCamera(float left, float right, float bottom, float top);

    void setPosition(const glm::vec3& position);
    void setRotation(float rotation);
    void setProjection(float left, float right, float bottom, float top);

    const glm::vec3& getPosition() const { return m_position; }
    float getRotation() const { return m_rotation; }

    const glm::mat4& getProjectionMatrix() const { return m_proj_mat; }
    const glm::mat4& getViewMatrix() const { return m_view_mat; }
    const glm::mat4& getVPMatrix() const { return m_vp_mat; }

private:
    void recalculateVPMatrix();

    glm::mat4 m_proj_mat{1.0f};
    glm::mat4 m_view_mat{1.0f};
    glm::mat4 m_vp_mat{1.0f};

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    float m_rotation{0.0f};
};

} // namespace GE

#endif // GE_RENDERER_ORTHOGRAPHIC_CAMERA_H_
