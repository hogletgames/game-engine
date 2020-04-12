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
#include "ge/imgui/imgui_layer.h"
#include "ge/layer.h"
#include "ge/window/window.h"
#include "ge/window/window_event.h"

namespace GE {

Application* Application::m_instance{nullptr};

Application::Application()
    : m_window(Window::create())
{
    GE_CORE_ASSERT(!m_instance, "Application already exists");
    m_instance = this;
    m_window->setEventCallback(GE_BIND_MEM_FN(Application::onEvent));

    m_imgui_layer = std::make_shared<ImGuiLayer>();
    pushOverlay(m_imgui_layer);
}

void Application::run()
{
    while (m_runnign) {
        for (auto& layer : m_layer_stack) {
            layer->onUpdate();
        }

        m_imgui_layer->begin();

        for (auto& layer : m_layer_stack) {
            layer->onImGuiRender();
        }

        m_imgui_layer->end();
        m_window->onUpdate();
    }
}

void Application::pushLayer(std::shared_ptr<Layer> layer)
{
    layer->onAttach();
    m_layer_stack.pushLayer(std::move(layer));
}

void Application::pushOverlay(std::shared_ptr<Layer> overlay)
{
    overlay->onAttach();
    m_layer_stack.pushOverlay(std::move(overlay));
}

void Application::onEvent(Event* event)
{
    EventDispatcher dispatcher{event};
    dispatcher.dispatch<WindowClosedEvent>(GE_BIND_MEM_FN(Application::onWindowClosed));

    for (auto layer = m_layer_stack.rbegin(); layer != m_layer_stack.rend(); ++layer) {
        (*layer)->onEvent(event);

        if (event->handled()) {
            break;
        }
    }
}

bool Application::onWindowClosed([[maybe_unused]] const WindowClosedEvent& event)
{
    m_runnign = false;
    return true;
}

} // namespace GE
