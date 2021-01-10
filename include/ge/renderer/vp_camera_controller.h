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

#ifndef GE_RENDERER_VP_CAMERA_CONTROLLER_H_
#define GE_RENDERER_VP_CAMERA_CONTROLLER_H_

#include <ge/core/timestamp.h>

#include <glm/glm.hpp>

namespace GE {

class Event;
class MouseScrolledEvent;
class ViewProjectionCamera;

class GE_API VPCameraController
{
public:
    VPCameraController() = default;
    explicit VPCameraController(Shared<ViewProjectionCamera> camera);

    void onUpdate(Timestamp dt);
    void onEvent(Event* event);

    void resize(const glm::vec2& size);

    void setCamera(Shared<ViewProjectionCamera> camera);
    const Shared<ViewProjectionCamera>& getCamera() const { return m_camera; }

private:
    bool onMouseScrolled(const MouseScrolledEvent& event);

    void mousePan(const glm::vec2& delta);
    void mouseRotate(const glm::vec2& delta);
    void mouseZoom(float delta);

    glm::vec2 getPanSpeed() const;
    float getZoomSpeed() const;

    Shared<ViewProjectionCamera> m_camera;
    glm::vec2 m_prev_mouse_pos{0.0f, 0.0f};
};

} // namespace GE

#endif // GE_RENDERER_VP_CAMERA_CONTROLLER_H_
