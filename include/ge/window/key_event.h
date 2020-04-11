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

#ifndef GE_WINDOW_KEY_EVENT_H_
#define GE_WINDOW_KEY_EVENT_H_

#include <ge/window/event.h>
#include <ge/window/key_codes.h>

#include <sstream>

namespace GE {

class GE_API KeyEvent: public Event
{
public:
    KeyCode getKeyCode() const { return m_key_code; }

protected:
    explicit KeyEvent(KeyCode key_code)
        : m_key_code{key_code}
    {}

    KeyCode m_key_code{GE_KEY_UNKNOWN};
};

class GE_API KeyPressedEvent: public KeyEvent
{
public:
    explicit KeyPressedEvent(KeyCode key_code = GE_KEY_UNKNOWN, uint32_t repeat_count = 0)
        : KeyEvent{key_code}
        , m_repeat_count{repeat_count}
    {}

    uint32_t getRepeatCount() const { return m_repeat_count; }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_key_code << " (" << m_repeat_count << " repeats)";
        return ss.str();
    }

    DECLARE_EVENT_TYPE(KEY_PRESSED)

private:
    uint32_t m_repeat_count{};
};

class GE_API KeyReleasedEvent: public KeyEvent
{
public:
    explicit KeyReleasedEvent(KeyCode key_code = GE_KEY_UNKNOWN)
        : KeyEvent{key_code}
    {}

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_key_code;
        return ss.str();
    }

    DECLARE_EVENT_TYPE(KEY_RELEASED)
};

class GE_API KeyTypedEvent: public Event
{
public:
    explicit KeyTypedEvent(const char* text = nullptr)
        : m_text{text}
    {}

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_text;
        return ss.str();
    }

    const char* getText() const { return m_text; }

    DECLARE_EVENT_TYPE(KEY_TYPED)

private:
    const char* m_text{nullptr};
};

} // namespace GE

#endif // GE_WINDOW_KEY_EVENT_H_
