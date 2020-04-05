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

#include "opengl_context.h"
#include "unix_utils.h"

#include <SDL.h>
#include <glad/glad.h>

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 5

namespace GE::UNIX {

OpenGLContext::OpenGLContext(void* window)
    : m_window{reinterpret_cast<SDL_Window*>(window)}
{
    GE_CORE_TRACE("OpenGL context has been created");
}

void OpenGLContext::initialize()
{
    SDLCall(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION));
    SDLCall(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION));
    SDLCall(
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

    m_gl_context = SDL_GL_CreateContext(m_window);
    GE_CORE_ASSERT(m_gl_context, "Failed to create graphics context");

    auto glad_load_proc = static_cast<GLADloadproc>(SDL_GL_GetProcAddress);
    int is_glad_loaded = gladLoadGLLoader(glad_load_proc);
    GE_CORE_ASSERT(is_glad_loaded, "Failed to initialize GLAD");

    GE_CORE_INFO("OpenGL Version: {}", glGetString(GL_VERSION));
    GE_CORE_INFO("OpenGL Shading Language Version: {}",
                 glGetString(GL_SHADING_LANGUAGE_VERSION));
    GE_CORE_INFO("OpenGL Vendor: {}", glGetString(GL_VENDOR));
    GE_CORE_INFO("OpenGL Renderer: {}", glGetString(GL_RENDERER));
}

void OpenGLContext::shutdown()
{
    SDL_GL_DeleteContext(m_gl_context);
    GE_CORE_TRACE("OpenGL context has been deleted");
}

void OpenGLContext::swapBuffers()
{
    SDL_GL_SwapWindow(m_window);
}

} // namespace GE::UNIX
