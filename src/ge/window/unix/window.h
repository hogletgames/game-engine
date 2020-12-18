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
#ifndef GE_WINDOW_UNIX_WINDOW_H_
#define GE_WINDOW_UNIX_WINDOW_H_

#include "ge/renderer/graphics_context.h"
#include "ge/window/window.h"

union SDL_Event;
struct SDL_Window;

namespace GE::UNIX {

class Window: public ::GE::Window
{
public:
    explicit Window(properties_t prop);
    ~Window() override;

    static bool initialize();
    static void shutdown();

    void setVSync(bool enabled) override;
    bool isVSync() const override { return m_prop.vsync; }

    void* getNativeWindow() const override { return m_window; };
    void* getNativeContext() const override { return m_context->getNativeContext(); }
    uint32_t getWidth() const override { return m_prop.width; }
    uint32_t getHeight() const override { return m_prop.height; }
    const properties_t& getProps() const override { return m_prop; }

    void onUpdate() override;
    void setEventCallback(WinEventCallback callback) override
    {
        m_event_callback = callback;
    }

private:
    void pollEvents();
    void onSDLMouseEvent(const SDL_Event& sdl_event);
    void onSDLKeyEvent(const SDL_Event& sdl_event);
    void onSDLWindowEvent(const SDL_Event& sdl_event);

    SDL_Window* m_window{nullptr};
    Scoped<GraphicsContext> m_context;

    WinEventCallback m_event_callback;
    properties_t m_prop;
};

} // namespace GE::UNIX

#endif // GE_WINDOW_UNIX_WINDOW_H_
