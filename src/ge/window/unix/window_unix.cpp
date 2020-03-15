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

#include "window_unix.h"
#include "utils_unix.h"

#include "ge/core/log.h"

#define VSYNC_OFF       0
#define VSYNC_ON        1
#define GE_GL_MAJOR_VER 4
#define GE_GL_MINOR_VER 5

namespace GE::priv {

bool WindowUnix::m_initialized{false};

WindowUnix::WindowUnix(const properties_t& prop)
    : m_prop(prop)
{
    GE_CORE_TRACE("Creating window '{}', ({}, {})", m_prop.title, m_prop.width,
                  m_prop.height);

    int pos_x = SDL_WINDOWPOS_CENTERED;
    int pos_y = SDL_WINDOWPOS_CENTERED;
    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN |
                   SDL_WINDOW_OPENGL;

    m_window = SDL_CreateWindow(m_prop.title.c_str(), pos_x, pos_y, m_prop.width,
                                m_prop.height, flags);
    GE_CORE_ASSERT(m_window, "Failed to create SDL window");

    m_gl_contex = SDL_GL_CreateContext(m_window);
    GE_CORE_ASSERT(m_gl_contex, "Failed to create GL context");

    SDLCall(SDL_GL_MakeCurrent(m_window, m_gl_contex));

    setVSync(true);
    GE_CORE_TRACE("Window '{}' created", m_prop.title);
}

WindowUnix::~WindowUnix()
{
    if (m_gl_contex) {
        SDL_GL_DeleteContext(m_gl_contex);
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
}

void WindowUnix::initialize()
{
    SDLCall(SDL_Init(SDL_INIT_VIDEO));
    SDLCall(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GE_GL_MAJOR_VER));
    SDLCall(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GE_GL_MINOR_VER));
    SDLCall(
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
}

void WindowUnix::shutdown()
{
    SDL_Quit();
    GE_CORE_TRACE("SDL quit");
}

void WindowUnix::setVSync(bool enabled)
{
    if (enabled) {
        SDLCall(SDL_GL_SetSwapInterval(VSYNC_ON));
    } else {
        SDLCall(SDL_GL_SetSwapInterval(VSYNC_OFF));
    }

    m_vsync = enabled;
}

void WindowUnix::onUpdate()
{
    // TODO poll events
    SDL_GL_SwapWindow(m_window);
}

} // namespace GE::priv
