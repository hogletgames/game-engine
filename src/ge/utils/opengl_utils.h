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

// NOLINTNEXTLINE
#ifndef GE_UTILS_OPENGL_UTILS_H_
#define GE_UTILS_OPENGL_UTILS_H_

#include "ge/core/asserts.h"

#include <glad/glad.h>

#if defined(GE_DEBUG)
    #define GLCall(gl_func)                                         \
        do {                                                        \
            ::GE::OpenGL::clearGlError();                           \
            (gl_func);                                              \
            if (::GE::OpenGL::checkGlError()) {                     \
                GE_CORE_ASSERT(false, "'{}' call error", #gl_func); \
            }                                                       \
        } while (false)
#else
    #define GLCall(gl_func) (gl_func)
#endif

namespace GE::OpenGL {

inline void clearGlError()
{
    constexpr size_t max_iter_count{1000000};

    for (size_t i{0}; i < max_iter_count; i++) {
        if (glGetError() == GL_NO_ERROR) {
            break;
        }
    }
}

inline bool checkGlError()
{
    int32_t error_code = glGetError();

    if (error_code == GL_NO_ERROR) {
        return false;
    }

    const char* error = "";
    const char* description = "";

    switch (error_code) {
        case GL_INVALID_ENUM:
            error = "GL_INVALID_ENUM";
            description = "An unacceptable value has been specified for an enumerated "
                          "argument.";
            break;

        case GL_INVALID_VALUE:
            error = "GL_INVALID_VALUE";
            description = "A numeric argument is out of range.";
            break;

        case GL_INVALID_OPERATION:
            error = "GL_INVALID_OPERATION";
            description = "The specified operation is not allowed in the current state.";
            break;

        case GL_STACK_OVERFLOW:
            error = "GL_STACK_OVERFLOW";
            description = "This command would cause a stack overflow.";
            break;

        case GL_STACK_UNDERFLOW:
            error = "GL_STACK_UNDERFLOW";
            description = "This command would cause a stack underflow.";
            break;

        case GL_OUT_OF_MEMORY:
            error = "GL_OUT_OF_MEMORY";
            description = "There is not enough memory left to execute the command.";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "GL_INVALID_FRAMEBUFFER_OPERATION";
            description = "The object bound to FRAMEBUFFER_BINDING is not framebuffer "
                          "complete.";
            break;

        case GL_CONTEXT_LOST:
            error = "GL_CONTEXT_LOST";
            description = "OpenGL context has been lost, due to a graphics card reset.";
            break;

        default:
            error = "Unknown error";
            description = "No description";
            break;
    }

    GE_CORE_ERR("{}: '{}'", error, description);

    return true;
}

} // namespace GE::OpenGL

#endif // GE_UTILS_OPENGL_UTILS_H_
