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

#include "gui.h"

#include "ge/application.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"
#include "ge/renderer/render_command.h"
#include "ge/window/input.h"
#include "ge/window/key_event.h"
#include "ge/window/mouse_event.h"
#include "ge/window/window_event.h"

#include <imgui.h>

#if defined(GE_PLATFORM_UNIX)
    #include "unix/gui.h"
using PlatformGui = ::GE::UNIX::Gui;
#else
    #error "Platform is not defined!"
#endif

#define CAST_KEY(key) static_cast<int>(key)

namespace {

constexpr auto FONT_OPEN_SANS_PATH = "fonts/OpenSans/OpenSans-Bold.ttf";
constexpr float FONT_OPEN_SANS_SIZE{18.0f};

int32_t toImGuiButton(GE::MouseButton button)
{
    switch (button) {
        case GE_BUTTON_LEFT: return 0;
        case GE_BUTTON_RIGHT: return 1;
        case GE_BUTTON_MIDDLE: return 2;
        case GE_BUTTON_X1: return 3;
        case GE_BUTTON_X2: return 4;
        default: return -1;
    }
}

void mapKeys()
{
    GE_PROFILE_FUNC();
    ImGuiIO& io = ImGui::GetIO();

    io.KeyMap[ImGuiKey_Tab] = CAST_KEY(GE_KEY_TAB);
    io.KeyMap[ImGuiKey_LeftArrow] = CAST_KEY(GE_KEY_LEFT);
    io.KeyMap[ImGuiKey_RightArrow] = CAST_KEY(GE_KEY_RIGHT);
    io.KeyMap[ImGuiKey_UpArrow] = CAST_KEY(GE_KEY_UP);
    io.KeyMap[ImGuiKey_DownArrow] = CAST_KEY(GE_KEY_DOWN);
    io.KeyMap[ImGuiKey_PageUp] = CAST_KEY(GE_KEY_PAGEUP);
    io.KeyMap[ImGuiKey_PageDown] = CAST_KEY(GE_KEY_PAGEDOWN);
    io.KeyMap[ImGuiKey_Home] = CAST_KEY(GE_KEY_HOME);
    io.KeyMap[ImGuiKey_End] = CAST_KEY(GE_KEY_END);
    io.KeyMap[ImGuiKey_Insert] = CAST_KEY(GE_KEY_INSERT);
    io.KeyMap[ImGuiKey_Delete] = CAST_KEY(GE_KEY_DELETE);
    io.KeyMap[ImGuiKey_Backspace] = CAST_KEY(GE_KEY_BACKSPACE);
    io.KeyMap[ImGuiKey_Space] = CAST_KEY(GE_KEY_SPACE);
    io.KeyMap[ImGuiKey_Enter] = CAST_KEY(GE_KEY_RETURN);
    io.KeyMap[ImGuiKey_Escape] = CAST_KEY(GE_KEY_ESCAPE);
    io.KeyMap[ImGuiKey_KeyPadEnter] = CAST_KEY(GE_KEY_KP_ENTER);
    io.KeyMap[ImGuiKey_A] = CAST_KEY(GE_KEY_A);
    io.KeyMap[ImGuiKey_C] = CAST_KEY(GE_KEY_C);
    io.KeyMap[ImGuiKey_V] = CAST_KEY(GE_KEY_V);
    io.KeyMap[ImGuiKey_X] = CAST_KEY(GE_KEY_X);
    io.KeyMap[ImGuiKey_Y] = CAST_KEY(GE_KEY_Y);
    io.KeyMap[ImGuiKey_Z] = CAST_KEY(GE_KEY_Z);
}

void setControlKeys()
{
    GE_PROFILE_FUNC();
    ImGuiIO& io = ImGui::GetIO();

    io.KeyAlt = io.KeysDown[CAST_KEY(GE_KEY_LALT)] || io.KeysDown[CAST_KEY(GE_KEY_RALT)];

    io.KeyCtrl = io.KeysDown[CAST_KEY(GE_KEY_LCTRL)] ||
                 io.KeysDown[CAST_KEY(GE_KEY_RCTRL)];

    io.KeyShift = io.KeysDown[CAST_KEY(GE_KEY_LSHIFT)] ||
                  io.KeysDown[CAST_KEY(GE_KEY_RSHIFT)];

    io.KeySuper = io.KeysDown[CAST_KEY(GE_KEY_LSUPER)] ||
                  io.KeysDown[CAST_KEY(GE_KEY_RSUPER)];
}

bool onKeyPressed(const GE::KeyPressedEvent& event)
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[CAST_KEY(event.getKeyCode())] = true;
    setControlKeys();
    return GE::Gui::isEventsBlocked() && io.WantCaptureKeyboard;
}

bool onKeyReleased(const GE::KeyReleasedEvent& event)
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[CAST_KEY(event.getKeyCode())] = false;
    setControlKeys();
    return GE::Gui::isEventsBlocked() && io.WantCaptureKeyboard;
}

bool onKeyTyped(const GE::KeyTypedEvent& event)
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharactersUTF8(event.getText());
    return GE::Gui::isEventsBlocked() && io.WantCaptureKeyboard;
}

bool onMouseMoved(const GE::MouseMovedEvent& event)
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(event.getPosX(), event.getPosY());
    return GE::Gui::isEventsBlocked() && io.WantCaptureMouse;
}

bool onMouseScrolled(const GE::MouseScrolledEvent& event)
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH = event.getOffsetX();
    io.MouseWheel = event.getOffsetY();
    return GE::Gui::isEventsBlocked() && io.WantCaptureMouse;
}

bool onMouseButtonPressed(const GE::MouseButtonPressedEvent& event)
{
    GE_PROFILE_FUNC();
    auto button = event.getMouseButton();

    if (button == GE_BUTTON_UNKNOWN) {
        return false;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[toImGuiButton(button)] = true;
    return GE::Gui::isEventsBlocked() && io.WantCaptureMouse;
}

bool onMouseButtonReleased(const GE::MouseButtonReleasedEvent& event)
{
    GE_PROFILE_FUNC();
    auto button = event.getMouseButton();

    if (button == GE_BUTTON_UNKNOWN) {
        return false;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[toImGuiButton(button)] = false;
    return GE::Gui::isEventsBlocked() && io.WantCaptureMouse;
}

bool onWindowResized(const GE::WindowResizedEvent& event)
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(event.getWidth(), event.getHeight());
    io.DisplayFramebufferScale = ImVec2{1.0f, 1.0f};
    GE::RenderCommand::setViewport(0, 0, event.getWidth(), event.getHeight());
    return false;
}

} // namespace

namespace GE {

bool Gui::initialize(const std::string& assets_dir)
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Initialize GUI");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    std::string font_path = assets_dir + "/" + FONT_OPEN_SANS_PATH;
    io.Fonts->AddFontFromFileTTF(font_path.c_str(), FONT_OPEN_SANS_SIZE);

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (!PlatformGui::initialize()) {
        GE_CORE_ERR("Failed to initialize Platform::GUI");
        return false;
    }

    mapKeys();
    return true;
}

void Gui::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Shutdown GUI");
    PlatformGui::shutdown();
    ImGui::DestroyContext();
}

void Gui::begin()
{
    GE_PROFILE_FUNC();

    PlatformGui::newFrame();
    ImGui::NewFrame();
}

void Gui::end()
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    const auto& window = Application::getWindow();
    io.DisplaySize = ImVec2(window.getWidth(), window.getHeight());

    ImGui::Render();
    PlatformGui::render();
}

void Gui::onEvent(Event* event)
{
    GE_PROFILE_FUNC();
    EventDispatcher dispatcher(event);

    dispatcher.dispatch<KeyPressedEvent>(onKeyPressed);
    dispatcher.dispatch<KeyReleasedEvent>(onKeyReleased);
    dispatcher.dispatch<KeyTypedEvent>(onKeyTyped);
    dispatcher.dispatch<MouseMovedEvent>(onMouseMoved);
    dispatcher.dispatch<MouseScrolledEvent>(onMouseScrolled);
    dispatcher.dispatch<MouseButtonPressedEvent>(onMouseButtonPressed);
    dispatcher.dispatch<MouseButtonReleasedEvent>(onMouseButtonReleased);
    dispatcher.dispatch<WindowResizedEvent>(onWindowResized);
}

} // namespace GE
