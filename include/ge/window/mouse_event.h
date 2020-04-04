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

#ifndef GE_WINDOW_MOUSE_EVENT_H_
#define GE_WINDOW_MOUSE_EVENT_H_

#include <ge/window/event.h>
#include <ge/window/mouse_button_codes.h>

#include <sstream>

namespace GE {

class GE_API MouseMovedEvent: public Event
{
public:
    MouseMovedEvent(float x = 0, float y = 0)
        : m_pos_x{x}
        , m_pos_y{y}
    {}

    float getPosX() const { return m_pos_x; }
    float getPosY() const { return m_pos_y; }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_pos_x << ", " << m_pos_y;
        return ss.str();
    }

    DECLARE_EVENT_TYPE(MOUSE_MOVED)

private:
    float m_pos_x{};
    float m_pos_y{};
};

class GE_API MouseScrolledEvent: public Event
{
public:
    MouseScrolledEvent(float offset_x = 0, float offset_y = 0)
        : m_offset_x{offset_x}
        , m_offset_y{offset_y}
    {}

    float getOffsetX() const { return m_offset_x; }
    float getOffsetY() const { return m_offset_y; }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << m_offset_x << ", " << m_offset_y;
        return ss.str();
    }

    DECLARE_EVENT_TYPE(MOUSE_SCROLLED)

private:
    float m_offset_x{};
    float m_offset_y{};
};

class GE_API MouseButtonEvent: public Event
{
public:
    MouseButton getMouseButton() const { return m_button; }

protected:
    explicit MouseButtonEvent(MouseButton button)
        : m_button{button}
    {}

    MouseButton m_button{};
};

class GE_API MouseButtonPressedEvent: public MouseButtonEvent
{
public:
    explicit MouseButtonPressedEvent(MouseButton button = GE_BUTTON_UNKNOWN)
        : MouseButtonEvent{button}
    {}

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_button;
        return ss.str();
    }

    DECLARE_EVENT_TYPE(MOUSE_BUTTON_PRESSED)
};

class GE_API MouseButtonReleasedEvent: public MouseButtonEvent
{
public:
    explicit MouseButtonReleasedEvent(MouseButton button = GE_BUTTON_UNKNOWN)
        : MouseButtonEvent{button}
    {}

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_button;
        return ss.str();
    }

    DECLARE_EVENT_TYPE(MOUSE_BUTTON_RELEASED)
};

} // namespace GE

#endif // GE_WINDOW_MOUSE_EVENT_H_
