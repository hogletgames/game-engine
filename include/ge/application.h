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

#include <ge/core/core.h>
#include <ge/layer_stack.h>
#include <ge/window/window.h>

#include <memory>

namespace GE {

class Event;
class WindowClosedEvent;

class GE_API Application
{
public:
    Application();
    virtual ~Application() = default;

    void run();

    void pushLayer(std::shared_ptr<Layer> layer);
    void pushOverlay(std::shared_ptr<Layer> overlay);

    const Window& getWindow() { return *m_window; }
    void* getNativeWindow() { return m_window->getNativeWindow(); }

    static Application& instance() { return *m_instance; }

private:
    void onEvent(Event& event);
    bool onWindowClosed(WindowClosedEvent& event);

    static Application* m_instance;

    std::unique_ptr<Window> m_window;
    LayerStack m_layer_stack;
    bool m_runnign{true};
};

} // namespace GE

#endif // GE_APPLICATION_H_
