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

#include "imgui_layer.h"

#include "ge/application.h"
#include "ge/core/log.h"
#include "ge/window/input.h"
#include "ge/window/key_event.h"
#include "ge/window/mouse_event.h"
#include "ge/window/window_event.h"

#include "imgui.h"

#if defined(GE_PLATFORM_UNIX)
    #include "unix/imgui_unix.h"
using ImGuiPlatform = ::GE::priv::ImGuiUnix;
#else
    #error "Unsupported platform"
#endif

namespace GE {

void ImGuiLayer::onAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGuiPlatform::initialize();
    GE_CORE_TRACE("'{}' attached", getName());
}

void ImGuiLayer::onDetach()
{
    ImGuiPlatform::shutdown();
    ImGui::DestroyContext();
    GE_CORE_TRACE("'{}' detached", getName());
}

void ImGuiLayer::onUpdate()
{
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::instance();
    io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

    io.DeltaTime = 1.0f / 60.0f;

    ImGuiPlatform::newFrame();
    ImGui::NewFrame();

    bool show_demo_window{true};
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGuiPlatform::render();
}

void ImGuiLayer::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.dispatch<KeyPressedEvent>(GE_BIND_MEM_FN(ImGuiLayer::onKeyPressed));
    dispatcher.dispatch<KeyReleasedEvent>(GE_BIND_MEM_FN(ImGuiLayer::onKeyReleased));
    dispatcher.dispatch<KeyTypedEvent>(GE_BIND_MEM_FN(ImGuiLayer::onKeyTyped));
    dispatcher.dispatch<MouseMovedEvent>(GE_BIND_MEM_FN(ImGuiLayer::onMouseMoved));
    dispatcher.dispatch<MouseScrolledEvent>(GE_BIND_MEM_FN(ImGuiLayer::onMouseScrolled));
    dispatcher.dispatch<MouseButtonPressedEvent>(
        GE_BIND_MEM_FN(ImGuiLayer::onMouseButtonPressed));
    dispatcher.dispatch<MouseButtonReleasedEvent>(
        GE_BIND_MEM_FN(ImGuiLayer::onMouseButtonReleased));
    dispatcher.dispatch<WindowResizedEvent>(GE_BIND_MEM_FN(ImGuiLayer::onWindowResized));
}

bool ImGuiLayer::onKeyPressed([[maybe_unused]] KeyPressedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[Input::toNativeKeyCode(event.getKeyCode())] = true;
    ImGuiPlatform::setControlKey();
    return false;
}

bool ImGuiLayer::onKeyReleased([[maybe_unused]] KeyReleasedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[Input::toNativeKeyCode(event.getKeyCode())] = false;
    ImGuiPlatform::setControlKey();
    return false;
}

bool ImGuiLayer::onKeyTyped(KeyTypedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharactersUTF8(event.getText());
    return false;
}

bool ImGuiLayer::onMouseMoved(MouseMovedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(event.getPosX(), event.getPosY());
    return false;
}

bool ImGuiLayer::onMouseScrolled(MouseScrolledEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH = event.getOffsetX();
    io.MouseWheel = event.getOffsetY();
    return false;
}

bool ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[Input::toNativeButton(event.getMouseButton()) - 1] = true;
    return false;
}

bool ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[Input::toNativeButton(event.getMouseButton()) - 1] = false;
    return false;
}

bool ImGuiLayer::onWindowResized([[maybe_unused]] WindowResizedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_size = ImVec2(event.getWidth(), event.getHeight());
    io.DisplaySize = window_size;
    io.DisplayFramebufferScale = ImVec2{1.0f, 1.0f};
    ImGuiPlatform::changeViewport(window_size);
    return false;
}

} // namespace GE
