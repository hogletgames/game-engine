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

#include "application.h"

#include "ge/core/asserts.h"
#include "ge/core/begin.h"
#include "ge/debug/profile.h"
#include "ge/gui/gui.h"
#include "ge/layer.h"
#include "ge/renderer/renderer.h"
#include "ge/window/window.h"
#include "ge/window/window_event.h"

namespace GE {
Application* Application::s_instance{nullptr};
Scoped<Window> Application::s_window{nullptr};

Application::Application()
{
    GE_PROFILE_FUNC();

    GE_CORE_ASSERT(!s_instance, "Application already exists");
    s_instance = this;
    s_window->setEventCallback([this](Event* event) { onEvent(event); });
}

Application::~Application()
{
    GE_PROFILE_FUNC();

    s_instance = nullptr;
}

void Application::initialize()
{
    GE_PROFILE_FUNC();

    GE_CORE_TRACE("Initialize Application");
    s_window = Window::create();
}

void Application::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_TRACE("Shutdown Application");
    s_window.reset();
}

void Application::run()
{
    GE_PROFILE_FUNC();
    m_prev_frame_time = Timestamp::now();

    while (m_runnign) {
        GE_PROFILE_SCOPE("MainLoop");

        Timestamp now = Timestamp::now();
        Timestamp delta_time = now - m_prev_frame_time;
        m_prev_frame_time = now;

        if (m_window_state != WindowState::MINIMIZED) {
            updateLayers(delta_time);
        }

        s_window->onUpdate();
    }
}

void Application::pushLayer(Shared<Layer> layer)
{
    GE_PROFILE_FUNC();

    layer->onAttach();
    m_layer_stack.pushLayer(std::move(layer));
}

void Application::pushOverlay(Shared<Layer> overlay)
{
    GE_PROFILE_FUNC();

    overlay->onAttach();
    m_layer_stack.pushOverlay(std::move(overlay));
}

void Application::updateLayers(Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    {
        GE_PROFILE_SCOPE("LayerStack onUpdate");

        for (auto& layer : m_layer_stack) {
            layer->onUpdate(delta_time);
        }
    }

    {
        GE_PROFILE_SCOPE("LayerStack onGuiRender");
        Begin<Gui> begin;

        for (auto& layer : m_layer_stack) {
            layer->onGuiRender();
        }
    }
}

void Application::onEvent(Event* event)
{
    GE_PROFILE_FUNC();

    EventDispatcher dispatcher{event};
    dispatcher.dispatch<WindowResizedEvent>(Renderer::onWindowResized);
    dispatcher.dispatch<WindowClosedEvent>(GE_BIND_EVENT_FN(onWindowClosed));
    dispatcher.dispatch<WindowMaximizedEvent>(GE_BIND_EVENT_FN(onWindowMaximized));
    dispatcher.dispatch<WindowMinimizedEvent>(GE_BIND_EVENT_FN(onWindowMinimized));
    dispatcher.dispatch<WindowRestoredEvent>(GE_BIND_EVENT_FN(onWindowRestored));

    Gui::onEvent(event);

    for (auto layer = m_layer_stack.rbegin(); layer != m_layer_stack.rend(); ++layer) {
        if (event->handled()) {
            break;
        }

        (*layer)->onEvent(event);
    }
}

bool Application::onWindowClosed([[maybe_unused]] const WindowClosedEvent& event)
{
    GE_PROFILE_FUNC();

    m_runnign = false;
    return true;
}

bool Application::onWindowMaximized([[maybe_unused]] const WindowMaximizedEvent& event)
{
    GE_PROFILE_FUNC();

    m_window_state = WindowState::MAXIMIZED;
    return false;
}

bool Application::onWindowMinimized([[maybe_unused]] const WindowMinimizedEvent& event)
{
    GE_PROFILE_FUNC();

    m_window_state = WindowState::MINIMIZED;
    return false;
}

bool Application::onWindowRestored([[maybe_unused]] const WindowRestoredEvent& event)
{
    GE_PROFILE_FUNC();

    m_window_state = WindowState::NONE;
    return false;
}

} // namespace GE
