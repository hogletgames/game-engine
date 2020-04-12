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

#include "platform_imgui.h"
#include "unix_utils.h"

#include "ge/application.h"
#include "ge/core/log.h"

#include <SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#define GLSL_VERSION "#version 450"

namespace GE::UNIX {

void PlatformImGui::initialize()
{
    GE_CORE_TRACE("Initialize Unix::PlatformImGui");

    void* window = Application::instance()->getNativeWindow();
    void* context = SDL_GL_GetCurrentContext();

    ImGui_ImplSDL2_InitForOpenGL(reinterpret_cast<SDL_Window*>(window), context);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void PlatformImGui::shutdown()
{
    GE_CORE_TRACE("Shutdown Unix::PlatformImGui");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}

void PlatformImGui::newFrame()
{
    void* window = Application::instance()->getNativeWindow();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(reinterpret_cast<SDL_Window*>(window));
}

void PlatformImGui::render()
{
    ImGuiIO& io = ImGui::GetIO();
    auto width = static_cast<int32_t>(io.DisplaySize.x);
    auto height = static_cast<int32_t>(io.DisplaySize.y);
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glViewport(0, 0, width, height);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDLCall(SDL_GL_MakeCurrent(backup_current_window, backup_current_context));
    }
}

void PlatformImGui::updateViewport(const ImVec2& window_size)
{
    glViewport(0, 0, window_size.x, window_size.y);
}

} // namespace GE::UNIX
