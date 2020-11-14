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

// NOLINTNEXTLINE
#ifndef GE_WINDOW_UNIX_INPUT_H_
#define GE_WINDOW_UNIX_INPUT_H_

#include "ge/window/input.h"
#include "ge/window/key_codes.h"
#include "ge/window/mouse_button_codes.h"

#include <unordered_map>

namespace GE::UNIX {

class InputImpl: public ::GE::InputImpl
{
public:
    bool initialize() override;
    void shutdown() override;

    int32_t toNativeKeyCode(KeyCode key_code) const override;
    KeyCode toGEKeyCode(int32_t key_code) const override;
    bool isKeyPressed(KeyCode key_code) const override;

    uint8_t toNativeButton(MouseButton button) const override;
    MouseButton toGEMouseButton(uint8_t button) const override;
    bool isMouseButtonPressed(MouseButton button) const override;
    glm::vec2 getMousePos() const override;

private:
    void mapKeyCodes();
    void mapMouseButtons();

    std::unordered_map<int32_t, KeyCode> m_sdl_to_ge_keys;
    std::unordered_map<KeyCode, int32_t> m_ge_to_sdl_keys;

    std::unordered_map<uint8_t, MouseButton> m_sdl_to_ge_buttons;
    std::unordered_map<MouseButton, uint8_t> m_ge_to_sdl_buttons;
};

} // namespace GE::UNIX

#endif // GE_WINDOW_UNIX_INPUT_H_
