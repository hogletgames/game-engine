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

#ifndef GE_WINDOW_INPUT_H_
#define GE_WINDOW_INPUT_H_

#include <ge/core/core.h>
#include <ge/window/key_codes.h>
#include <ge/window/mouse_button_codes.h>

#include <cstdint>
#include <memory>
#include <utility>

namespace GE {

class GE_API Input
{
public:
    Input(const Input& other) = delete;
    Input(Input&& other) = delete;

    Input& operator=(const Input& other) = delete;
    Input& operator=(Input&& other) = delete;

    virtual ~Input() = default;

    static void initialize();
    static void shutdown();

    static int32_t toNativeKeyCode(KeyCode key_code)
    {
        return m_impl->toNativeKeyCodeImpl(key_code);
    }

    static KeyCode toGEKeyCode(int32_t key_code)
    {
        return m_impl->toGEKeyCodeImpl(key_code);
    }

    static bool isKeyPressed(KeyCode key_code)
    {
        return m_impl->isKeyPressedImpl(key_code);
    }

    static uint8_t toNativeButton(MouseButton button)
    {
        return m_impl->toNativeButtonImpl(button);
    }

    static MouseButton toGEMouseButton(uint8_t button)
    {
        return m_impl->toGEMouseButtonImpl(button);
    }

    static bool isMouseButtonPressed(MouseButton button)
    {
        return m_impl->isMouseButtonPressedImpl(button);
    }

    static std::pair<float, float> getMousePos() { return m_impl->getMousePosImpl(); }
    static float getMousePosX() { return m_impl->getMousePosXImpl(); }
    static float getMousePosY() { return m_impl->getMousePosYImpl(); }

protected:
    Input() = default;

    virtual void initializeImpl() = 0;
    virtual void shutdownImpl() = 0;

    virtual int32_t toNativeKeyCodeImpl(KeyCode key_code) const = 0;
    virtual KeyCode toGEKeyCodeImpl(int32_t key_code) const = 0;
    virtual bool isKeyPressedImpl(KeyCode key_code) const = 0;

    virtual uint8_t toNativeButtonImpl(MouseButton button) const = 0;
    virtual MouseButton toGEMouseButtonImpl(uint8_t button) const = 0;
    virtual bool isMouseButtonPressedImpl(MouseButton button) const = 0;
    virtual std::pair<float, float> getMousePosImpl() const = 0;
    virtual float getMousePosXImpl() const = 0;
    virtual float getMousePosYImpl() const = 0;

private:
    static Scoped<Input> m_impl;
};

} // namespace GE

#endif // GE_WINDOW_INPUT_H_
