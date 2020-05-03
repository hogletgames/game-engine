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

#include "ortho_camera_controller.h"

#include "ge/debug/profile.h"
#include "ge/window/input.h"
#include "ge/window/mouse_event.h"
#include "ge/window/window_event.h"

#define CAMERA_ROTATION_SPEED 180.0f
#define ZOOM_OFFSET_SCALE     0.25f
#define ZOOM_MIN              0.25f

#define ROTATION_180DEG 180.0f
#define ROTATION_360DEG 360.0f

namespace GE {

OrthoCameraController::OrthoCameraController(float aspect_ratio, bool rotation)
    : m_aspect_ratio{aspect_ratio}
    , m_camera{-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom}
    , m_rotation_enable(rotation)
{}

void OrthoCameraController::onUpdate(Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    auto delta_time_sec = static_cast<float>(delta_time.sec());

    if (Input::isKeyPressed(GE_KEY_D)) {
        m_camera_pos.x += cos(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
        m_camera_pos.y += sin(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
    } else if (Input::isKeyPressed(GE_KEY_A)) {
        m_camera_pos.x -= cos(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
        m_camera_pos.y -= sin(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
    }

    if (Input::isKeyPressed(GE_KEY_W)) {
        m_camera_pos.x += -sin(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
        m_camera_pos.y += cos(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
    } else if (Input::isKeyPressed(GE_KEY_S)) {
        m_camera_pos.x -= -sin(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
        m_camera_pos.y -= cos(glm::radians(m_camera_rotation)) *
                          m_camera_translation_speed * delta_time_sec;
    }

    if (m_rotation_enable) {
        if (Input::isKeyPressed(GE_KEY_E)) {
            m_camera_rotation += CAMERA_ROTATION_SPEED * delta_time_sec;
        } else if (Input::isKeyPressed(GE_KEY_Q)) {
            m_camera_rotation -= CAMERA_ROTATION_SPEED * delta_time_sec;
        }

        if (m_camera_rotation >= ROTATION_180DEG) {
            m_camera_rotation -= ROTATION_360DEG;
        } else if (m_camera_rotation <= -ROTATION_180DEG) {
            m_camera_rotation += ROTATION_360DEG;
        }

        m_camera.setRotation(m_camera_rotation);
    }

    m_camera.setPosition(m_camera_pos);
    m_camera_translation_speed = m_zoom;
}

void OrthoCameraController::onEvent(Event *event)
{
    GE_PROFILE_FUNC();

    EventDispatcher dispatcher{event};
    dispatcher.dispatch<MouseScrolledEvent>(GE_BIND_EVENT_FN(onMouseScrolled));
    dispatcher.dispatch<WindowResizedEvent>(GE_BIND_EVENT_FN(onWindowResizedEvent));
}

bool OrthoCameraController::onMouseScrolled(const MouseScrolledEvent &event)
{
    GE_PROFILE_FUNC();

    m_zoom -= event.getOffsetY() * ZOOM_OFFSET_SCALE;
    m_zoom = std::max(m_zoom, ZOOM_MIN);
    m_camera.setProjection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom,
                           m_zoom);
    return false;
}

bool OrthoCameraController::onWindowResizedEvent(const WindowResizedEvent &event)
{
    GE_PROFILE_FUNC();

    m_aspect_ratio = static_cast<float>(event.getWidth()) / event.getHeight();
    m_camera.setProjection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom,
                           m_zoom);
    return false;
}

} // namespace GE
