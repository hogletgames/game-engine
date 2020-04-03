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

#ifndef GE_WINDOW_KEY_CODES_H_
#define GE_WINDOW_KEY_CODES_H_

#include <iostream>

#define GE_KEY_UNKNOWN ::GE::KeyCode::UNKNOWN

#define GE_KEY_BACKSPACE ::GE::KeyCode::BACKSPACE
#define GE_KEY_TAB       ::GE::KeyCode::TAB
#define GE_KEY_RETURN    ::GE::KeyCode::RETURN
#define GE_KEY_EXCAPE    ::GE::KeyCode::EXCAPE

#define GE_KEY_SPACE      ::GE::KeyCode::SPACE
#define GE_KEY_EXCLAIM    ::GE::KeyCode::EXCLAIM
#define GE_KEY_QUOTEDBL   ::GE::KeyCode::QUOTEDBL
#define GE_KEY_HASH       ::GE::KeyCode::HASH
#define GE_KEY_PERCENT    ::GE::KeyCode::PERCENT
#define GE_KEY_DOLLAR     ::GE::KeyCode::DOLLAR
#define GE_KEY_AMPERSAND  ::GE::KeyCode::AMPERSAND
#define GE_KEY_QUOTE      ::GE::KeyCode::QUOTE
#define GE_KEY_LEFTPAREN  ::GE::KeyCode::LEFTPAREN
#define GE_KEY_RIGHTPAREN ::GE::KeyCode::RIGHTPAREN
#define GE_KEY_ASTERISK   ::GE::KeyCode::ASTERISK
#define GE_KEY_PLUS       ::GE::KeyCode::PLUS
#define GE_KEY_COMMA      ::GE::KeyCode::COMMA
#define GE_KEY_MINUS      ::GE::KeyCode::MINUS
#define GE_KEY_PERIOD     ::GE::KeyCode::PERIOD
#define GE_KEY_SLASH      ::GE::KeyCode::SLASH

#define GE_KEY_D0 ::GE::KeyCode::D0
#define GE_KEY_D1 ::GE::KeyCode::D1
#define GE_KEY_D2 ::GE::KeyCode::D2
#define GE_KEY_D3 ::GE::KeyCode::D3
#define GE_KEY_D4 ::GE::KeyCode::D4
#define GE_KEY_D5 ::GE::KeyCode::D5
#define GE_KEY_D6 ::GE::KeyCode::D6
#define GE_KEY_D7 ::GE::KeyCode::D7
#define GE_KEY_D8 ::GE::KeyCode::D8
#define GE_KEY_D9 ::GE::KeyCode::D9

#define GE_KEY_COLON     ::GE::KeyCode::COLON
#define GE_KEY_SEMICOLON ::GE::KeyCode::SEMICOLON
#define GE_KEY_LESS      ::GE::KeyCode::LESS
#define GE_KEY_EQUALS    ::GE::KeyCode::EQUALS
#define GE_KEY_GREATER   ::GE::KeyCode::GREATER
#define GE_KEY_QUESTION  ::GE::KeyCode::QUESTION
#define GE_KEY_AT        ::GE::KeyCode::AT

#define GE_KEY_A ::GE::KeyCode::A
#define GE_KEY_B ::GE::KeyCode::B
#define GE_KEY_C ::GE::KeyCode::C
#define GE_KEY_D ::GE::KeyCode::D
#define GE_KEY_E ::GE::KeyCode::E
#define GE_KEY_F ::GE::KeyCode::F
#define GE_KEY_G ::GE::KeyCode::G
#define GE_KEY_H ::GE::KeyCode::H
#define GE_KEY_I ::GE::KeyCode::I
#define GE_KEY_J ::GE::KeyCode::J
#define GE_KEY_K ::GE::KeyCode::K
#define GE_KEY_L ::GE::KeyCode::L
#define GE_KEY_M ::GE::KeyCode::M
#define GE_KEY_N ::GE::KeyCode::N
#define GE_KEY_O ::GE::KeyCode::O
#define GE_KEY_P ::GE::KeyCode::P
#define GE_KEY_Q ::GE::KeyCode::Q
#define GE_KEY_R ::GE::KeyCode::R
#define GE_KEY_S ::GE::KeyCode::S
#define GE_KEY_T ::GE::KeyCode::T
#define GE_KEY_U ::GE::KeyCode::U
#define GE_KEY_V ::GE::KeyCode::V
#define GE_KEY_W ::GE::KeyCode::W
#define GE_KEY_X ::GE::KeyCode::X
#define GE_KEY_Y ::GE::KeyCode::Y
#define GE_KEY_Z ::GE::KeyCode::Z

#define GE_KEY_LEFTBRACKET  ::GE::KeyCode::LEFTBRACKET
#define GE_KEY_BACKSLASH    ::GE::KeyCode::BACKSLASH
#define GE_KEY_RIGHTBRACKET ::GE::KeyCode::RIGHTBRACKET
#define GE_KEY_CARET        ::GE::KeyCode::CARET
#define GE_KEY_UNDERSCORE   ::GE::KeyCode::UNDERSCORE
#define GE_KEY_BACKQUOTE    ::GE::KeyCode::BACKQUOTE

#define GE_KEY_ESCAPE      ::GE::KeyCode::ESCAPE
#define GE_KEY_ENTER       ::GE::KeyCode::ENTER
#define GE_KEY_TAB         ::GE::KeyCode::TAB
#define GE_KEY_BACKSPACE   ::GE::KeyCode::BACKSPACE
#define GE_KEY_INSERT      ::GE::KeyCode::INSERT
#define GE_KEY_DELETE      ::GE::KeyCode::DELETE
#define GE_KEY_RIGHT       ::GE::KeyCode::RIGHT
#define GE_KEY_LEFT        ::GE::KeyCode::LEFT
#define GE_KEY_DOWN        ::GE::KeyCode::DOWN
#define GE_KEY_UP          ::GE::KeyCode::UP
#define GE_KEY_PAGEUP      ::GE::KeyCode::PAGEUP
#define GE_KEY_PAGEDOWN    ::GE::KeyCode::PAGEDOWN
#define GE_KEY_HOME        ::GE::KeyCode::HOME
#define GE_KEY_END         ::GE::KeyCode::END
#define GE_KEY_CAPSLOCK    ::GE::KeyCode::CAPSLOCK
#define GE_KEY_SCROLLLOCK  ::GE::KeyCode::SCROLLLOCK
#define GE_KEY_NUMLOCK     ::GE::KeyCode::NUMLOCK
#define GE_KEY_PRINTSCREEN ::GE::KeyCode::PRINTSCREEN
#define GE_KEY_PAUSE       ::GE::KeyCode::PAUSE

#define GE_KEY_F1  ::GE::KeyCode::F1
#define GE_KEY_F2  ::GE::KeyCode::F2
#define GE_KEY_F3  ::GE::KeyCode::F3
#define GE_KEY_F4  ::GE::KeyCode::F4
#define GE_KEY_F5  ::GE::KeyCode::F5
#define GE_KEY_F6  ::GE::KeyCode::F6
#define GE_KEY_F7  ::GE::KeyCode::F7
#define GE_KEY_F8  ::GE::KeyCode::F8
#define GE_KEY_F9  ::GE::KeyCode::F9
#define GE_KEY_F10 ::GE::KeyCode::F10
#define GE_KEY_F11 ::GE::KeyCode::F11
#define GE_KEY_F12 ::GE::KeyCode::F12
#define GE_KEY_F13 ::GE::KeyCode::F13
#define GE_KEY_F14 ::GE::KeyCode::F14
#define GE_KEY_F15 ::GE::KeyCode::F15
#define GE_KEY_F16 ::GE::KeyCode::F16
#define GE_KEY_F17 ::GE::KeyCode::F17
#define GE_KEY_F18 ::GE::KeyCode::F18
#define GE_KEY_F19 ::GE::KeyCode::F19
#define GE_KEY_F20 ::GE::KeyCode::F20
#define GE_KEY_F21 ::GE::KeyCode::F21
#define GE_KEY_F22 ::GE::KeyCode::F22
#define GE_KEY_F23 ::GE::KeyCode::F23
#define GE_KEY_F24 ::GE::KeyCode::F24
#define GE_KEY_F25 ::GE::KeyCode::F25

#define GE_KEY_KP0         ::GE::KeyCode::KP0
#define GE_KEY_KP1         ::GE::KeyCode::KP1
#define GE_KEY_KP2         ::GE::KeyCode::KP2
#define GE_KEY_KP3         ::GE::KeyCode::KP3
#define GE_KEY_KP4         ::GE::KeyCode::KP4
#define GE_KEY_KP5         ::GE::KeyCode::KP5
#define GE_KEY_KP6         ::GE::KeyCode::KP6
#define GE_KEY_KP7         ::GE::KeyCode::KP7
#define GE_KEY_KP8         ::GE::KeyCode::KP8
#define GE_KEY_KP9         ::GE::KeyCode::KP9
#define GE_KEY_KP_DECIMAL  ::GE::KeyCode::KP_DECIMAL
#define GE_KEY_KP_DIVIDE   ::GE::KeyCode::KP_DIVIDE
#define GE_KEY_KP_MULTIPLY ::GE::KeyCode::KP_MULTIPLY
#define GE_KEY_KP_MINUS    ::GE::KeyCode::KP_MINUS
#define GE_KEY_KP_PLUS     ::GE::KeyCode::KP_PLUS
#define GE_KEY_KP_ENTER    ::GE::KeyCode::KP_ENTER
#define GE_KEY_KP_EQUAL    ::GE::KeyCode::KP_EQUAL

#define GE_KEY_LSHIFT ::GE::KeyCode::LSHIFT
#define GE_KEY_LCTRL  ::GE::KeyCode::LCTRL
#define GE_KEY_LALT   ::GE::KeyCode::LALT
#define GE_KEY_LSUPER ::GE::KeyCode::LSUPER
#define GE_KEY_RSHIFT ::GE::KeyCode::RSHIFT
#define GE_KEY_RCTRL  ::GE::KeyCode::RCTRL
#define GE_KEY_RALT   ::GE::KeyCode::RALT
#define GE_KEY_RSUPER ::GE::KeyCode::RSUPER
#define GE_KEY_MENU   ::GE::KeyCode::MENU

namespace GE {

enum class KeyCode : uint16_t
{
    UNKNOWN = 0x00,

    BACKSPACE = 0x08,
    TAB = 0x09,
    RETURN = 0x0D,
    ESCAPE = 0x33,

    SPACE = ' ',
    EXCLAIM = '!',
    QUOTEDBL = '"',
    HASH = '#',
    PERCENT = '%',
    DOLLAR = '$',
    AMPERSAND = '&',
    QUOTE = '\'',
    LEFTPAREN = '(',
    RIGHTPAREN = ')',
    ASTERISK = '*',
    PLUS = '+',
    COMMA = ',',
    MINUS = '-',
    PERIOD = '.',
    SLASH = '/',

    D0 = '0',
    D1 = '1',
    D2 = '2',
    D3 = '3',
    D4 = '4',
    D5 = '5',
    D6 = '6',
    D7 = '7',
    D8 = '8',
    D9 = '9',

    COLON = ':',
    SEMICOLON = ';',
    LESS = '<',
    EQUALS = '=',
    GREATER = '>',
    QUESTION = '?',
    AT = '@',

    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    F = 'F',
    G = 'G',
    H = 'H',
    I = 'I',
    J = 'J',
    K = 'K',
    L = 'L',
    M = 'M',
    N = 'N',
    O = 'O',
    P = 'P',
    Q = 'Q',
    R = 'R',
    S = 'S',
    T = 'T',
    U = 'U',
    V = 'V',
    W = 'W',
    X = 'X',
    Y = 'Y',
    Z = 'Z',

    LEFTBRACKET = '[',
    BACKSLASH = '\\',
    RIGHTBRACKET = ']',
    CARET = '^',
    UNDERSCORE = '_',
    BACKQUOTE = '`',

    ENTER = 0x0101,
    INSERT = 0x0104,
    DELETE = 0x0105,
    RIGHT = 0x0106,
    LEFT = 0x0107,
    DOWN = 0x0108,
    UP = 0x0109,
    PAGEUP = 0x010A,
    PAGEDOWN = 0x010B,
    HOME = 0x010C,
    END = 0x010D,
    CAPSLOCK = 0x010E,
    SCROLLLOCK = 0x010F,
    NUMLOCK = 0x0110,
    PRINTSCREEN = 0x0111,
    PAUSE = 0x0112,

    F1 = 0x0113,
    F2 = 0x0114,
    F3 = 0x0115,
    F4 = 0x0116,
    F5 = 0x0117,
    F6 = 0x0118,
    F7 = 0x0119,
    F8 = 0x011A,
    F9 = 0x011B,
    F10 = 0x011C,
    F11 = 0x011D,
    F12 = 0x011E,
    F13 = 0x011F,
    F14 = 0x0120,
    F15 = 0x0121,
    F16 = 0x0122,
    F17 = 0x0123,
    F18 = 0x0124,
    F19 = 0x0125,
    F20 = 0x0126,
    F21 = 0x0127,
    F22 = 0x0128,
    F23 = 0x0129,
    F24 = 0x012A,
    F25 = 0x012B,

    KP0 = 0x012C,
    KP1 = 0x012D,
    KP2 = 0x012E,
    KP3 = 0x012f,
    KP4 = 0x0130,
    KP5 = 0x0131,
    KP6 = 0x0132,
    KP7 = 0x0133,
    KP8 = 0x0134,
    KP9 = 0x0135,
    KP_DECIMAL = 0x0136,
    KP_DIVIDE = 0x0137,
    KP_MULTIPLY = 0x0138,
    KP_MINUS = 0x0139,
    KP_PLUS = 0x013A,
    KP_ENTER = 0x013B,
    KP_EQUAL = 0x013C,

    LSHIFT = 0x013D,
    LCTRL = 0x013E,
    LALT = 0x013F,
    LSUPER = 0x0140,
    RSHIFT = 0x0141,
    RCTRL = 0x0142,
    RALT = 0x0143,
    RSUPER = 0x0144,
    MENU = 0x0145
};

} // namespace GE

inline std::ostream& operator<<(std::ostream& os, GE::KeyCode key_code)
{
    return os << static_cast<int>(key_code);
}

#endif // GE_WINDOW_KEY_CODES_H_
