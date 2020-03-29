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

#ifndef GE_WINDOWS_EVENT_H_
#define GE_WINDOWS_EVENT_H_

#include <functional>
#include <iostream>

#include <ge/core/core.h>

#define DECLARE_EVENT_TYPE(type)                                                 \
    static ::GE::Event::Type getStaticType() { return ::GE::Event::Type::type; } \
    ::GE::Event::Type getType() const override { return getStaticType(); }       \
    const char* getName() const override { return #type; }

namespace GE {

template<typename EventType>
using EventCallback = std::function<bool(EventType&)>;

class GE_API Event
{
public:
    enum class Type : uint8_t
    {
        NONE = 0,
        // Key events
        KEY_PRESSED,
        KEY_RELEASED,
        KEY_TYPED,
        // Mouse events
        MOUSE_MOVED,
        MOUSE_SCROLLED,
        MOUSE_BUTTON_PRESSED,
        MOUSE_BUTTON_RELEASED,
        // Window events
        WINDOW_RESIZED,
        WINDOW_CLOSED
    };

    virtual ~Event() = default;

    virtual Type getType() const = 0;
    virtual const char* getName() const = 0;
    virtual std::string toString() const { return getName(); }

    bool handled() const { return m_handled; }
    void setHandled(bool handled) { m_handled = handled; }

private:
    bool m_handled{false};
};

class GE_API EventDispatcher
{
public:
    explicit EventDispatcher(Event& event)
        : m_event{event}
    {}

    template<typename EventType>
    bool dispatch(const EventCallback<EventType>& event_callback)
    {
        if (m_event.getType() == EventType::getStaticType()) {
            EventType& event = static_cast<EventType&>(m_event);
            m_event.setHandled(event_callback(event));
            return true;
        }

        return false;
    }

private:
    Event& m_event;
};

} // namespace GE

inline std::ostream& operator<<(std::ostream& os, const GE::Event& event)
{
    return os << event.toString();
}

#endif // GE_WINDOWS_EVENT_H_
