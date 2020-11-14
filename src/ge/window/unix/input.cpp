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

#include "input.h"

#include "ge/core/log.h"
#include "ge/debug/profile.h"

#include <SDL.h>

#include <vector>

namespace GE::UNIX {

bool Input::initializeImpl()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Initialize UNIX::Input");
    mapKeyCodes();
    mapMouseButtons();
    return true;
}

void Input::shutdownImpl()
{
    GE_PROFILE_FUNC();
    GE_CORE_DBG("Shutdown UNIX::Input");
}

int32_t Input::toNativeKeyCodeImpl(KeyCode key_code) const
{
    GE_PROFILE_FUNC();

    auto key = m_ge_to_sdl_keys.find(key_code);
    return key != m_ge_to_sdl_keys.end() ? key->second : SDLK_UNKNOWN;
}

KeyCode Input::toGEKeyCodeImpl(int32_t key_code) const
{
    GE_PROFILE_FUNC();

    auto key = m_sdl_to_ge_keys.find(key_code);
    return key != m_sdl_to_ge_keys.end() ? key->second : GE_KEY_UNKNOWN;
}

bool Input::isKeyPressedImpl(KeyCode key_code) const
{
    GE_PROFILE_FUNC();

    const uint8_t* keys_state = SDL_GetKeyboardState(nullptr);
    SDL_Keycode sdl_key = toNativeKeyCodeImpl(key_code);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(sdl_key);
    return keys_state != nullptr ? keys_state[scancode] != 0 : false;
}

uint8_t Input::toNativeButtonImpl(MouseButton button_code) const
{
    GE_PROFILE_FUNC();

    auto button = m_ge_to_sdl_buttons.find(button_code);
    return button != m_ge_to_sdl_buttons.end() ? button->second : 0;
}

MouseButton Input::toGEMouseButtonImpl(uint8_t button_code) const
{
    GE_PROFILE_FUNC();

    auto button = m_sdl_to_ge_buttons.find(button_code);
    return button != m_sdl_to_ge_buttons.end() ? button->second : GE_BUTTON_UNKNOWN;
}

bool Input::isMouseButtonPressedImpl(MouseButton button) const
{
    GE_PROFILE_FUNC();

    uint8_t sdl_button = toNativeButtonImpl(button);
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(sdl_button); // NOLINT
}

glm::vec2 Input::getMousePosImpl() const
{
    GE_PROFILE_FUNC();

    int pos_x{0};
    int pos_y{0};
    SDL_GetMouseState(&pos_x, &pos_y);
    return {pos_x, pos_y};
}

void Input::mapKeyCodes()
{
    GE_PROFILE_FUNC();

    m_sdl_to_ge_keys = {{SDLK_UNKNOWN, GE_KEY_UNKNOWN},

                        {SDLK_BACKSPACE, GE_KEY_BACKSPACE},
                        {SDLK_TAB, GE_KEY_TAB},
                        {SDLK_RETURN, GE_KEY_RETURN},
                        {SDLK_ESCAPE, GE_KEY_ESCAPE},

                        {SDLK_SPACE, GE_KEY_SPACE},
                        {SDLK_EXCLAIM, GE_KEY_EXCLAIM},
                        {SDLK_QUOTEDBL, GE_KEY_QUOTEDBL},
                        {SDLK_HASH, GE_KEY_HASH},
                        {SDLK_PERCENT, GE_KEY_PERCENT},
                        {SDLK_DOLLAR, GE_KEY_DOLLAR},
                        {SDLK_AMPERSAND, GE_KEY_AMPERSAND},
                        {SDLK_QUOTE, GE_KEY_QUOTE},
                        {SDLK_LEFTPAREN, GE_KEY_LEFTPAREN},
                        {SDLK_RIGHTPAREN, GE_KEY_RIGHTPAREN},
                        {SDLK_ASTERISK, GE_KEY_ASTERISK},
                        {SDLK_PLUS, GE_KEY_PLUS},
                        {SDLK_COMMA, GE_KEY_COMMA},
                        {SDLK_MINUS, GE_KEY_MINUS},
                        {SDLK_PERIOD, GE_KEY_PERIOD},
                        {SDLK_SLASH, GE_KEY_SLASH},

                        {SDLK_0, GE_KEY_D0},
                        {SDLK_1, GE_KEY_D1},
                        {SDLK_2, GE_KEY_D2},
                        {SDLK_3, GE_KEY_D3},
                        {SDLK_4, GE_KEY_D4},
                        {SDLK_5, GE_KEY_D5},
                        {SDLK_6, GE_KEY_D6},
                        {SDLK_7, GE_KEY_D7},
                        {SDLK_8, GE_KEY_D8},
                        {SDLK_9, GE_KEY_D9},

                        {SDLK_COLON, GE_KEY_COLON},
                        {SDLK_SEMICOLON, GE_KEY_SEMICOLON},
                        {SDLK_LESS, GE_KEY_LESS},
                        {SDLK_EQUALS, GE_KEY_EQUALS},
                        {SDLK_GREATER, GE_KEY_GREATER},
                        {SDLK_QUESTION, GE_KEY_QUESTION},
                        {SDLK_AT, GE_KEY_AT},

                        {SDLK_a, GE_KEY_A},
                        {SDLK_b, GE_KEY_B},
                        {SDLK_c, GE_KEY_C},
                        {SDLK_d, GE_KEY_D},
                        {SDLK_e, GE_KEY_E},
                        {SDLK_f, GE_KEY_F},
                        {SDLK_g, GE_KEY_G},
                        {SDLK_h, GE_KEY_H},
                        {SDLK_i, GE_KEY_I},
                        {SDLK_j, GE_KEY_J},
                        {SDLK_k, GE_KEY_K},
                        {SDLK_l, GE_KEY_L},
                        {SDLK_m, GE_KEY_M},
                        {SDLK_n, GE_KEY_N},
                        {SDLK_o, GE_KEY_O},
                        {SDLK_p, GE_KEY_P},
                        {SDLK_q, GE_KEY_Q},
                        {SDLK_r, GE_KEY_R},
                        {SDLK_s, GE_KEY_S},
                        {SDLK_t, GE_KEY_T},
                        {SDLK_u, GE_KEY_U},
                        {SDLK_v, GE_KEY_V},
                        {SDLK_w, GE_KEY_W},
                        {SDLK_x, GE_KEY_X},
                        {SDLK_y, GE_KEY_Y},
                        {SDLK_z, GE_KEY_Z},

                        {SDLK_LEFTBRACKET, GE_KEY_LEFTBRACKET},
                        {SDLK_BACKSLASH, GE_KEY_BACKSLASH},
                        {SDLK_RIGHTBRACKET, GE_KEY_RIGHTBRACKET},
                        {SDLK_CARET, GE_KEY_CARET},
                        {SDLK_UNDERSCORE, GE_KEY_UNDERSCORE},
                        {SDLK_BACKQUOTE, GE_KEY_BACKQUOTE},

                        {SDLK_ESCAPE, GE_KEY_ESCAPE},
                        {SDLK_RETURN, GE_KEY_ENTER},
                        {SDLK_INSERT, GE_KEY_INSERT},
                        {SDLK_DELETE, GE_KEY_DELETE},
                        {SDLK_RIGHT, GE_KEY_RIGHT},
                        {SDLK_LEFT, GE_KEY_LEFT},
                        {SDLK_DOWN, GE_KEY_DOWN},
                        {SDLK_UP, GE_KEY_UP},
                        {SDLK_PAGEUP, GE_KEY_PAGEUP},
                        {SDLK_PAGEDOWN, GE_KEY_PAGEDOWN},
                        {SDLK_HOME, GE_KEY_HOME},
                        {SDLK_END, GE_KEY_END},
                        {SDLK_CAPSLOCK, GE_KEY_CAPSLOCK},
                        {SDLK_SCROLLLOCK, GE_KEY_SCROLLLOCK},
                        {SDLK_NUMLOCKCLEAR, GE_KEY_NUMLOCK},
                        {SDLK_PRINTSCREEN, GE_KEY_PRINTSCREEN},
                        {SDLK_PAUSE, GE_KEY_PAUSE},

                        {SDLK_F1, GE_KEY_F1},
                        {SDLK_F2, GE_KEY_F2},
                        {SDLK_F3, GE_KEY_F3},
                        {SDLK_F4, GE_KEY_F4},
                        {SDLK_F5, GE_KEY_F5},
                        {SDLK_F6, GE_KEY_F6},
                        {SDLK_F7, GE_KEY_F7},
                        {SDLK_F8, GE_KEY_F8},
                        {SDLK_F9, GE_KEY_F9},
                        {SDLK_F10, GE_KEY_F10},
                        {SDLK_F11, GE_KEY_F11},
                        {SDLK_F12, GE_KEY_F12},
                        {SDLK_F13, GE_KEY_F13},
                        {SDLK_F14, GE_KEY_F14},
                        {SDLK_F15, GE_KEY_F15},
                        {SDLK_F16, GE_KEY_F16},
                        {SDLK_F17, GE_KEY_F17},
                        {SDLK_F18, GE_KEY_F18},
                        {SDLK_F19, GE_KEY_F19},
                        {SDLK_F20, GE_KEY_F20},
                        {SDLK_F21, GE_KEY_F21},
                        {SDLK_F22, GE_KEY_F22},
                        {SDLK_F23, GE_KEY_F23},
                        {SDLK_F24, GE_KEY_F24},

                        {SDLK_KP_0, GE_KEY_KP0},
                        {SDLK_KP_1, GE_KEY_KP1},
                        {SDLK_KP_2, GE_KEY_KP2},
                        {SDLK_KP_3, GE_KEY_KP3},
                        {SDLK_KP_4, GE_KEY_KP4},
                        {SDLK_KP_5, GE_KEY_KP5},
                        {SDLK_KP_6, GE_KEY_KP6},
                        {SDLK_KP_7, GE_KEY_KP7},
                        {SDLK_KP_8, GE_KEY_KP8},
                        {SDLK_KP_9, GE_KEY_KP9},
                        {SDLK_KP_DECIMAL, GE_KEY_KP_DECIMAL},
                        {SDLK_KP_DIVIDE, GE_KEY_KP_DIVIDE},
                        {SDLK_KP_MULTIPLY, GE_KEY_KP_MULTIPLY},
                        {SDLK_KP_MINUS, GE_KEY_KP_MINUS},
                        {SDLK_KP_PLUS, GE_KEY_KP_PLUS},
                        {SDLK_KP_ENTER, GE_KEY_KP_ENTER},
                        {SDLK_KP_EQUALS, GE_KEY_KP_EQUAL},

                        {SDLK_LSHIFT, GE_KEY_LSHIFT},
                        {SDLK_LCTRL, GE_KEY_LCTRL},
                        {SDLK_LALT, GE_KEY_LALT},
                        {SDLK_LGUI, GE_KEY_LSUPER},
                        {SDLK_RSHIFT, GE_KEY_RSHIFT},
                        {SDLK_RCTRL, GE_KEY_RCTRL},
                        {SDLK_RALT, GE_KEY_RALT},
                        {SDLK_RGUI, GE_KEY_RSUPER},
                        {SDLK_MENU, GE_KEY_MENU}};

    for (auto [sdl_key, ge_key] : m_sdl_to_ge_keys) {
        m_ge_to_sdl_keys.emplace(ge_key, sdl_key);
    }
}

void Input::mapMouseButtons()
{
    GE_PROFILE_FUNC();

    m_sdl_to_ge_buttons = {{SDL_BUTTON_LEFT, GE_BUTTON_LEFT},
                           {SDL_BUTTON_RIGHT, GE_BUTTON_RIGHT},
                           {SDL_BUTTON_MIDDLE, GE_BUTTON_MIDDLE},
                           {SDL_BUTTON_X1, GE_BUTTON_X1},
                           {SDL_BUTTON_X2, GE_BUTTON_X2}};

    for (auto [sdl_button, ge_button] : m_sdl_to_ge_buttons) {
        m_ge_to_sdl_buttons.emplace(ge_button, sdl_button);
    }
}

} // namespace GE::UNIX
