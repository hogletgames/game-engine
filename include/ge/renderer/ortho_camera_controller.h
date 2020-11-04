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

#ifndef GE_RENDERER_ORTHO_CAMERA_CONTROLLER_H_
#define GE_RENDERER_ORTHO_CAMERA_CONTROLLER_H_

#include <ge/core/core.h>
#include <ge/core/timestamp.h>
#include <ge/renderer/orthographic_camera.h>

#include <glm/glm.hpp>

namespace GE {

class Event;
class MouseScrolledEvent;
class WindowResizedEvent;

class GE_API OrthoCameraController
{
public:
    explicit OrthoCameraController(float aspect_ratio, bool rotation = false);

    void onUpdate(Timestamp delta_time);
    void onEvent(Event* event);

    const OrthographicCamera& getCamera() const { return m_camera; }

    void setZoom(float zoom) { m_zoom = zoom; }
    float getZoom() const { return m_zoom; }

private:
    bool onMouseScrolled(const MouseScrolledEvent& event);
    bool onWindowResizedEvent(const WindowResizedEvent& event);

    float m_aspect_ratio{};
    float m_zoom{1.0f};
    OrthographicCamera m_camera;

    bool m_rotation_enable{false};
    glm::vec3 m_camera_pos{0.0f, 0.0f, 0.0f};
    float m_camera_rotation{0.0f};
    float m_camera_translation_speed{1.0f};
};

} // namespace GE

#endif // GE_RENDERER_ORTHO_CAMERA_CONTROLLER_H_
