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

#include "vp_camera_controller.h"
#include "view_projection_camera.h"

#include "ge/debug/profile.h"
#include "ge/window/input.h"
#include "ge/window/mouse_event.h"

namespace {

constexpr float MOUSE_MOVE_FACTOR{0.003f};
constexpr float ZOOM_DISTANCE_FACTOR{0.2f};
constexpr float MOUSE_SCROLL_FACTOR{0.1f};
constexpr float PAN_MAX{2400.f};
constexpr float PAN_COORD_FACTOR{1e-3};
constexpr float DISTANCE_MIN{0.0f};
constexpr float MOUSE_SPEED_MAX{100.0f};
constexpr float MOUSE_ROTATION_SPEED{0.8f};

} // namespace

namespace GE {

VPCameraController::VPCameraController(Shared<ViewProjectionCamera> camera)
    : m_camera{std::move(camera)}
{
    GE_PROFILE_FUNC();
}

void VPCameraController::onUpdate([[maybe_unused]] Timestamp dt)
{
    GE_PROFILE_FUNC();

    if (GE::Input::isKeyPressed(GE::KeyCode::LALT)) {
        auto mouse_pos = GE::Input::getMousePos();
        auto delta = (mouse_pos - m_prev_mouse_pos) * MOUSE_MOVE_FACTOR;
        m_prev_mouse_pos = mouse_pos;

        if (GE::Input::isMouseButtonPressed(GE::MouseButton::BUTTON_MIDDLE)) {
            mousePan(delta);
        } else if (GE::Input::isMouseButtonPressed(GE::MouseButton::BUTTON_LEFT)) {
            mouseRotate(delta);
        } else if (GE::Input::isMouseButtonPressed(GE::MouseButton::BUTTON_RIGHT)) {
            mouseZoom(delta.y);
        }
    }
}

void VPCameraController::onEvent(Event *event)
{
    GE_PROFILE_FUNC();

    EventDispatcher dispatcher{event};
    dispatcher.dispatch<MouseScrolledEvent>(GE_BIND_EVENT_FN(onMouseScrolled));
}

void VPCameraController::resize(const glm::vec2 &size)
{
    GE_PROFILE_FUNC();

    m_camera->setViewport(size);
    m_camera->calculateVPMatrix();
}

void VPCameraController::setCamera(Shared<ViewProjectionCamera> camera)
{
    GE_PROFILE_FUNC();

    m_camera = std::move(camera);
}

bool VPCameraController::onMouseScrolled(const MouseScrolledEvent &event)
{
    GE_PROFILE_FUNC();

    float delta = event.getOffsetY() * MOUSE_SCROLL_FACTOR;
    mouseZoom(delta);
    return false;
}

void VPCameraController::mousePan(const glm::vec2 &delta)
{
    GE_PROFILE_FUNC();

    auto speed = getPanSpeed();
    float distance = m_camera->getDistance();
    auto focal_point = m_camera->getFocalPoint();

    focal_point += -m_camera->getRightDirection() * delta.x * speed.x * distance;
    focal_point += m_camera->getUpDirection() * delta.y * speed.y * distance;

    m_camera->setFocalPoint(focal_point);
}

void VPCameraController::mouseRotate(const glm::vec2 &delta)
{
    GE_PROFILE_FUNC();

    float yaw_sign = m_camera->getUpDirection().y < 0.0f ? -1.0f : 1.0f;
    auto angles = glm::eulerAngles(m_camera->getOrientation());

    angles[0] -= delta.y * MOUSE_ROTATION_SPEED;            // pitch
    angles[1] -= yaw_sign * delta.x * MOUSE_ROTATION_SPEED; // yaw
    angles[2] = 0.0f;                                       // roll

    m_camera->setOrientation(angles);
}

void VPCameraController::mouseZoom(float delta)
{
    GE_PROFILE_FUNC();

    float distance = m_camera->getDistance() - (delta * getZoomSpeed());

    if (distance < 1.0f) {
        auto focal_point = m_camera->getFocalPoint();
        m_camera->setFocalPoint(focal_point + m_camera->getForwardDirection());
        distance = 1.0f;
    }

    m_camera->setDistance(distance);
}

glm::vec2 VPCameraController::getPanSpeed() const
{
    GE_PROFILE_FUNC();

    static constexpr float a{0.0366f};
    static constexpr float b{0.1778f};
    static constexpr float c{0.3021f};

    const auto &viewport = m_camera->getViewport();
    float x = std::min(viewport.x, PAN_MAX) * PAN_COORD_FACTOR;
    float y = std::min(viewport.y, PAN_MAX) * PAN_COORD_FACTOR;

    glm::vec2 factors;
    factors.x = a * (x * x) - b * x + c;
    factors.y = a * (y * y) - b * y + c;

    return factors;
}

float VPCameraController::getZoomSpeed() const
{
    GE_PROFILE_FUNC();

    float distance = m_camera->getDistance() * ZOOM_DISTANCE_FACTOR;
    distance = std::max(distance, DISTANCE_MIN);
    float speed = distance * distance;
    speed = std::min(speed, MOUSE_SPEED_MAX);
    return speed;
}

} // namespace GE
