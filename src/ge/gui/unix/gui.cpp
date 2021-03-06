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
#include "opengl_utils.h"
#include "unix_utils.h"

#include "ge/application.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"
#include "ge/renderer/render_command.h"

#include <SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#define GLSL_VERSION "#version 450"

namespace GE::UNIX {

bool Gui::initialize()
{
    GE_PROFILE_FUNC();
    GE_CORE_DBG("Initialize Unix::PlatformImGui");

    void* window = Application::getWindow().getNativeWindow();
    void* context = Application::getWindow().getNativeContext();

    if (!ImGui_ImplSDL2_InitForOpenGL(reinterpret_cast<SDL_Window*>(window), context)) {
        GE_CORE_ERR("Failed to initialize SDL2+OpenGL");
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init(GLSL_VERSION)) {
        GE_CORE_ERR("Failed to initialize OpenGL3");
        return false;
    }

    return true;
}

void Gui::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Shutdown Unix::PlatformImGui");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}

void Gui::newFrame()
{
    GE_PROFILE_FUNC();
    void* window = Application::getWindow().getNativeWindow();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(reinterpret_cast<SDL_Window*>(window));
}

void Gui::render()
{
    GE_PROFILE_FUNC();

    ImGuiIO& io = ImGui::GetIO();
    auto width = static_cast<int32_t>(io.DisplaySize.x);
    auto height = static_cast<int32_t>(io.DisplaySize.y);

    RenderCommand::setViewport(0, 0, width, height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDLCall(SDL_GL_MakeCurrent(backup_current_window, backup_current_context));
    }
}

} // namespace GE::UNIX
