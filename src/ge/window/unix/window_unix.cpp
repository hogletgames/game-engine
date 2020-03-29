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
#include "key_event.h"
#include "mouse_event.h"
#include "utils_unix.h"
#include "window_event.h"

#include "ge/core/log.h"

#include "glad/glad.h"

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
    GE_CORE_ASSERT(m_window, SDL_GetError());

    m_gl_contex = SDL_GL_CreateContext(m_window);
    GE_CORE_ASSERT(m_gl_contex, SDL_GetError());

    SDLCall(SDL_GL_MakeCurrent(m_window, m_gl_contex));

    auto glad_load_proc = static_cast<GLADloadproc>(SDL_GL_GetProcAddress);
    GE_CORE_ASSERT(gladLoadGLLoader(glad_load_proc), "Failed to initialize GLAD");

    GE_CORE_INFO("OpenGL Version: {}.{}", GLVersion.major, GLVersion.minor);
    GE_CORE_INFO("OpenGL SHading Language Version: {}",
                 glGetString(GL_SHADING_LANGUAGE_VERSION));
    GE_CORE_INFO("OpenGL Vendor: {}", glGetString(GL_VENDOR));
    GE_CORE_INFO("OpenGL Renderer: {}", glGetString(GL_RENDERER));

    setVSync(true);
    GE_CORE_TRACE("Window '{}' created", m_prop.title);
}

WindowUnix::~WindowUnix()
{
    if (m_gl_contex) {
        SDL_GL_DeleteContext(m_gl_contex);
        GE_CORE_TRACE("SDL GL context has been deleted");
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        GE_CORE_TRACE("SDL window '{}' has been destroyed", m_prop.title);
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
    pollEvents();
    SDL_GL_SwapWindow(m_window);
}

void WindowUnix::pollEvents()
{
    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event) != 0) {
        switch (sdl_event.type) {
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                onSDLMouseEvent(sdl_event);
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                onSDLKeyEvent(sdl_event);
                break;

            case SDL_WINDOWEVENT:
                onSDLWindowEvent(sdl_event);
                break;

            case SDL_QUIT: {
                WindowClosedEvent event{};
                m_event_callback(event);
                break;
            }

            default:
                break;
        }
    }
}

void WindowUnix::onSDLMouseEvent(const SDL_Event& sdl_event)
{
    switch (sdl_event.type) {
        case SDL_MOUSEMOTION: {
            float x = sdl_event.motion.x;
            float y = sdl_event.motion.y;
            MouseMovedEvent event{x, y};
            m_event_callback(event);
            break;
        }

        case SDL_MOUSEWHEEL: {
            float offset_x = sdl_event.wheel.x;
            float offset_y = sdl_event.wheel.y;
            MouseScrolledEvent event{offset_x, offset_y};
            m_event_callback(event);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            uint8_t button = sdl_event.button.button;
            MouseButtonPressedEvent event{button};
            m_event_callback(event);
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            uint8_t button = sdl_event.button.button;
            MouseButtonReleasedEvent event{button};
            m_event_callback(event);
            break;
        }

        default:
            break;
    }
}

void WindowUnix::onSDLKeyEvent(const SDL_Event& sdl_event)
{
    switch (sdl_event.type) {
        case SDL_KEYDOWN: {
            uint16_t code = sdl_event.key.keysym.scancode;
            uint32_t repeat_count = sdl_event.key.repeat;
            KeyPressedEvent event{code, repeat_count};
            m_event_callback(event);
            break;
        }

        case SDL_KEYUP: {
            uint16_t code = sdl_event.key.keysym.scancode;
            KeyReleasedEvent event{code};
            m_event_callback(event);
            break;
        }

        default:
            break;
    }
}

void WindowUnix::onSDLWindowEvent(const SDL_Event& sdl_event)
{
    switch (sdl_event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
            uint32_t width = sdl_event.window.data1;
            uint32_t height = sdl_event.window.data2;
            WindowResizedEvent event{width, height};
            m_event_callback(event);
            break;
        }

        case SDL_WINDOWEVENT_CLOSE: {
            WindowClosedEvent event{};
            m_event_callback(event);
            break;
        }

        default:
            break;
    }
}

} // namespace GE::priv
