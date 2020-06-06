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

#include "orthographic_camera.h"

#include "ge/debug/profile.h"

#include <glm/gtc/matrix_transform.hpp>

#define Z_NEAR (-1.0f)
#define Z_FAR  1.0f

namespace GE {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
{
    GE_PROFILE_FUNC();

    setProjection(left, right, bottom, top);
}

void OrthographicCamera::setPosition(const glm::vec3& position)
{
    GE_PROFILE_FUNC();

    m_position = position;
    recalculateVPMatrix();
}

void OrthographicCamera::setRotation(float rotation)
{
    GE_PROFILE_FUNC();

    m_rotation = rotation;
    recalculateVPMatrix();
}

void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
{
    GE_PROFILE_FUNC();

    m_proj_mat = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    m_vp_mat = m_proj_mat * m_view_mat;
}

void OrthographicCamera::recalculateVPMatrix()
{
    GE_PROFILE_FUNC();

    auto transform = glm::translate(glm::mat4{1.0f}, m_position);
    transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3{0, 0, 1});

    m_view_mat = glm::inverse(transform);
    m_vp_mat = m_proj_mat * m_view_mat;
}

} // namespace GE
