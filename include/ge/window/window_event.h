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

#ifndef GE_WINDOW_WINDOW_EVENT_H_
#define GE_WINDOW_WINDOW_EVENT_H_

#include <ge/window/event.h>

#include <sstream>

namespace GE {

class GE_API WindowResizedEvent: public Event
{
public:
    explicit WindowResizedEvent(uint32_t width = 0, uint32_t height = 0)
        : m_width{width}
        , m_height{height}
    {}

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "WindowResizedEvent: " << m_width << ", " << m_height;
        return ss.str();
    }

    DECLARE_EVENT_TYPE(WINDOW_RESIZED)

private:
    uint32_t m_width{};
    uint32_t m_height{};
};

class GE_API WindowClosedEvent: public Event
{
public:
    WindowClosedEvent() = default;

    std::string toString() const override { return "WindowClosedEvent:"; }

    DECLARE_EVENT_TYPE(WINDOW_CLOSED)
};

class GE_API WindowMaximizedEvent: public Event
{
public:
    std::string toString() const override { return "WindowMaximizedEvent"; }

    DECLARE_EVENT_TYPE(WINDOW_MAXIMIZED)
};

class GE_API WindowMinimizedEvent: public Event
{
public:
    std::string toString() const override { return "WindowMinimizedEvent"; }

    DECLARE_EVENT_TYPE(WINDOW_MINIMIZED)
};

class GE_API WindowRestoredEvent: public Event
{
public:
    std::string toString() const override { return "WindowRestoredEvent"; }

    DECLARE_EVENT_TYPE(WINDOW_RESTORED)
};

} // namespace GE

#endif // GE_WINDOW_WINDOW_EVENT_H_
