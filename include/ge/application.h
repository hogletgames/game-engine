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

#ifndef GE_APPLICATION_H_
#define GE_APPLICATION_H_

#include "ge/core/timestamp.h"
#include <ge/core/non_copyable.h>
#include <ge/layer_stack.h>
#include <ge/window/window.h>

#include <memory>

namespace GE {

class Event;
class WindowResizedEvent;
class WindowClosedEvent;
class WindowMinimizedEvent;
class WindowMaximizedEvent;
class WindowRestoredEvent;

class GE_API Application
{
public:
    static bool initialize();
    static void shutdown();

    static void run();

    static void pushLayer(Shared<Layer> layer);
    static void pushOverlay(Shared<Layer> overlay);

    static const Window& getWindow() { return *get()->m_window; }

private:
    Application() = default;

    enum class WindowState : uint8_t
    {
        NONE = 0,
        MAXIMIZED,
        MINIMIZED
    };

    static Application* get()
    {
        static Application instance;
        return &instance;
    }

    void mainLoop();

    void updateLayers(Timestamp delta_time);

    void onEvent(Event* event);
    bool onWindowClosed(const WindowClosedEvent& event);
    bool onWindowMaximized(const WindowMaximizedEvent& event);
    bool onWindowMinimized(const WindowMinimizedEvent& event);
    bool onWindowRestored(const WindowRestoredEvent& event);

    Scoped<Window> m_window;
    WindowState m_window_state{WindowState::NONE};

    LayerStack m_layer_stack;
    bool m_runnign{true};
    Timestamp m_prev_frame_time;
};

} // namespace GE

#endif // GE_APPLICATION_H_
