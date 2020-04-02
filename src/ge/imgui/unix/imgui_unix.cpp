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

#include "imgui_unix.h"
#include "imgui.h"

#include "ge/application.h"
#include "ge/core/log.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_sdl.h"

#include "glad/glad.h"
#include "SDL.h"

#define GLSL_VERSION "#version 450"

namespace GE::priv {

void ImGuiUnix::initialize()
{
    GE_CORE_TRACE("Initialize ImGuiUnix");
    auto* window = static_cast<SDL_Window*>(Application::instance().getNativeWindow());

    ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void ImGuiUnix::shutdown()
{
    GE_CORE_TRACE("Shutdown ImGuiUnix");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}

void ImGuiUnix::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
}

void ImGuiUnix::render()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiUnix::changeViewport(const ImVec2& window_size)
{
    glViewport(0, 0, window_size.x, window_size.y);
}

void ImGuiUnix::setControlKey()
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeyCtrl = io.KeysDown[SDL_SCANCODE_LCTRL] || io.KeysDown[SDL_SCANCODE_RCTRL];
    io.KeyShift = io.KeysDown[SDL_SCANCODE_LSHIFT] || io.KeysDown[SDL_SCANCODE_RSHIFT];
    io.KeyAlt = io.KeysDown[SDL_SCANCODE_LALT] || io.KeysDown[SDL_SCANCODE_RALT];
    io.KeySuper = io.KeysDown[SDL_SCANCODE_LGUI] || io.KeysDown[SDL_SCANCODE_RGUI];
}

} // namespace GE::priv
