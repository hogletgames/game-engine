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

#include "window.h"
#include "input.h"
#include "unix_utils.h"

#include "ge/debug/profile.h"
#include "ge/renderer/graphics_context.h"
#include "ge/renderer/renderer.h"
#include "ge/window/key_event.h"
#include "ge/window/mouse_event.h"
#include "ge/window/window_event.h"

#include <SDL.h>
#include <glad/glad.h>

#define VSYNC_OFF 0
#define VSYNC_ON  1

namespace GE::UNIX {

bool Window::s_initialized{false};

Window::Window(properties_t prop)
    : m_prop(std::move(prop))
{
    GE_PROFILE_FUNC();
    GE_CORE_TRACE("Create window '{}', ({}, {})", m_prop.title, m_prop.width,
                  m_prop.height);

    int32_t pos_x = SDL_WINDOWPOS_CENTERED;
    int32_t pos_y = SDL_WINDOWPOS_CENTERED;
    uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN;

    if (Renderer::getAPI() == GE_OPEN_GL_API) {
        flags |= SDL_WINDOW_OPENGL;
    }

    {
        GE_PROFILE_SCOPE("UNIX::Window Create Window");
        m_window = SDL_CreateWindow(m_prop.title.c_str(), pos_x, pos_y, m_prop.width,
                                    m_prop.height, flags);
        GE_CORE_ASSERT(m_window, SDL_GetError());
    }

    m_contex = GraphicsContext::create(m_window);
    GE_CORE_ASSERT(m_contex != nullptr, "Failed to create graphics context");

    SDLCall(SDL_GL_MakeCurrent(m_window, m_contex->getNativeContext()));
    m_contex->initialize();

    GE_CORE_TRACE("Window '{}' created", m_prop.title);
}

Window::~Window() // NOLINT
{
    GE_PROFILE_FUNC();

    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        GE_CORE_TRACE("SDL window '{}' has been destroyed", m_prop.title);
    }

    if (m_contex != nullptr) {
        m_contex->shutdown();
    }
}

void Window::initialize()
{
    GE_PROFILE_FUNC();

    GE_CORE_TRACE("Initialize UNIX::Window");
    SDLCall(SDL_Init(SDL_INIT_VIDEO));
}

void Window::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_TRACE("Shutdown UNIX::Window");
    SDL_Quit();
}

void Window::setVSync(bool enabled)
{
    GE_PROFILE_FUNC();

    if (enabled) {
        SDLCall(SDL_GL_SetSwapInterval(VSYNC_ON));
    } else {
        SDLCall(SDL_GL_SetSwapInterval(VSYNC_OFF));
    }

    m_vsync = enabled;
}

void Window::onUpdate()
{
    GE_PROFILE_FUNC();

    pollEvents();
    m_contex->swapBuffers();
}

void Window::pollEvents()
{
    GE_PROFILE_FUNC();

    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event) != 0) {
        switch (sdl_event.type) {
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                onSDLMouseEvent(sdl_event);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_TEXTINPUT: {
                onSDLKeyEvent(sdl_event);
                break;
            }
            case SDL_WINDOWEVENT: {
                onSDLWindowEvent(sdl_event);
                break;
            }
            case SDL_QUIT: {
                WindowClosedEvent event{};
                m_event_callback(&event);
                break;
            }
            default: break;
        }
    }
}

void Window::onSDLMouseEvent(const SDL_Event& sdl_event)
{
    GE_PROFILE_FUNC();

    switch (sdl_event.type) {
        case SDL_MOUSEMOTION: {
            float x = sdl_event.motion.x;
            float y = sdl_event.motion.y;
            MouseMovedEvent event{x, y};
            m_event_callback(&event);
            break;
        }
        case SDL_MOUSEWHEEL: {
            float offset_x = sdl_event.wheel.x;
            float offset_y = sdl_event.wheel.y;
            MouseScrolledEvent event{offset_x, offset_y};
            m_event_callback(&event);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            MouseButton button = Input::toGEMouseButton(sdl_event.button.button);
            MouseButtonPressedEvent event{button};
            m_event_callback(&event);
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            MouseButton button = Input::toGEMouseButton(sdl_event.button.button);
            MouseButtonReleasedEvent event{button};
            m_event_callback(&event);
            break;
        }
        default: break;
    }
}

void Window::onSDLKeyEvent(const SDL_Event& sdl_event)
{
    GE_PROFILE_FUNC();

    switch (sdl_event.type) {
        case SDL_KEYDOWN: {
            KeyCode code = Input::toGEKeyCode(sdl_event.key.keysym.sym);
            uint32_t repeat_count = sdl_event.key.repeat;
            KeyPressedEvent event{code, repeat_count};
            m_event_callback(&event);
            break;
        }
        case SDL_KEYUP: {
            KeyCode code = Input::toGEKeyCode(sdl_event.key.keysym.sym);
            KeyReleasedEvent event{code};
            m_event_callback(&event);
            break;
        }
        case SDL_TEXTINPUT: {
            KeyTypedEvent event{sdl_event.text.text};
            m_event_callback(&event);
            break;
        }
        default: break;
    }
}

void Window::onSDLWindowEvent(const SDL_Event& sdl_event)
{
    GE_PROFILE_FUNC();

    switch (sdl_event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
            uint32_t width = sdl_event.window.data1;
            uint32_t height = sdl_event.window.data2;
            m_prop.width = width;
            m_prop.height = height;
            WindowResizedEvent event{width, height};
            m_event_callback(&event);
            break;
        }
        case SDL_WINDOWEVENT_CLOSE: {
            WindowClosedEvent event{};
            m_event_callback(&event);
            break;
        }
        case SDL_WINDOWEVENT_MAXIMIZED: {
            WindowMaximizedEvent event{};
            m_event_callback(&event);
            break;
        }
        case SDL_WINDOWEVENT_MINIMIZED: {
            WindowMinimizedEvent event{};
            m_event_callback(&event);
            break;
        }
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_RESTORED: {
            WindowRestoredEvent event{};
            m_event_callback(&event);
            break;
        }
        default: break;
    }
}

} // namespace GE::UNIX
