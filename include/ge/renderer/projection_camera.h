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

#ifndef GE_RENDERER_PROJECTION_CAMERA_H_
#define GE_RENDERER_PROJECTION_CAMERA_H_

#include <ge/core/interface.h>

#include <glm/glm.hpp>

namespace GE {

class GE_API ProjectionCamera: public Interface
{
public:
    enum class ProjectionType : uint8_t
    {
        UNKNOWN = 0,
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    struct perspective_params_t {
        float fov{FOV_DEFAULT};
        float near{NEAR_DEFAULT};
        float far{FAR_DEFAULT};

        static constexpr float FOV_DEFAULT{45.0f};
        static constexpr float NEAR_DEFAULT{0.01f};
        static constexpr float FAR_DEFAULT{1000.0f};
    };

    struct orthographic_params_t {
        float size{SIZE_DEFAULT};
        float near{NEAR_DEFAULT};
        float far{FAR_DEFAULT};

        static constexpr float SIZE_DEFAULT{10.f};
        static constexpr float NEAR_DEFAULT{-1.0f};
        static constexpr float FAR_DEFAULT{1.0f};
    };

    ProjectionCamera();

    const glm::mat4& getProjectionMatrix() const { return m_proj_mat; }

    void setViewport(const glm::vec2& viewport);
    const glm::vec2& getViewport() const { return m_viewport; }

    ProjectionType getProjectionType() const { return m_projection_type; }
    void setProjectionType(ProjectionType projection_type);

    const perspective_params_t& getPerspective() const { return m_perspective; }
    void setPerspectiveParams(const perspective_params_t& perspective);

    const orthographic_params_t& getOrthographic() const { return m_orthographic; }
    void setOrthographicParams(const orthographic_params_t& orthographic);

    void calculateProjection();

private:
    void calculatePerspectiveProjection();
    void calculateOrthographicProjection();

    float getAspectRatio() const { return std::abs(m_viewport.x / m_viewport.y); }

    ProjectionType m_projection_type{ProjectionType::ORTHOGRAPHIC};
    glm::vec2 m_viewport{1.0f, 1.0f};
    glm::mat4 m_proj_mat{1.0f};

    perspective_params_t m_perspective{};
    orthographic_params_t m_orthographic{};
};

std::string toString(ProjectionCamera::ProjectionType projection);
ProjectionCamera::ProjectionType toCameraProjection(const std::string& projection);

} // namespace GE

#endif // GE_RENDERER_PROJECTION_CAMERA_H_
