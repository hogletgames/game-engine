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

#ifndef GE_WINDOW_UNIX_UTILS_H_
#define GE_WINDOW_UNIX_UTILS_H_

#include "ge/core/core.h"

#define MOUSE_BUTTON_LEFT   0
#define MOUSE_BUTTON_RIGHT  1
#define MOUSE_BUTTON_MIDDLE 2
#define MOUSE_BUTTON_X1     3
#define MOUSE_BUTTON_X2     4

#define GE_CONVERT_MOUSE_BUTTON(button)           \
    {                                             \
        if (button == SDL_BUTTON_LEFT) {          \
            button = MOUSE_BUTTON_LEFT;           \
        } else if (button == SDL_BUTTON_RIGHT) {  \
            button = MOUSE_BUTTON_RIGHT;          \
        } else if (button == SDL_BUTTON_MIDDLE) { \
            button = MOUSE_BUTTON_MIDDLE;         \
        }                                         \
    }

#if defined(GE_DEBUG)
    #define SDLCall(x)                                                            \
        {                                                                         \
            GE_CORE_ASSERT((x) != -1, "'{}' call error: {}", #x, SDL_GetError()); \
        }
#else
    #define SDLCall(x) (x)
#endif

#endif // GE_WINDOW_UNIX_UTILS_H_
