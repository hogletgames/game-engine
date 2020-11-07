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

#include "shader.h"
#include "opengl_utils.h"

#include "ge/core/asserts.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"

#include <glad/glad.h>

#include <fstream>

namespace {

GLenum toGlType(::GE::Shader::Type type)
{
    switch (type) {
        case GE_VERTEX_SHADER: return GL_VERTEX_SHADER;
        case GE_FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
        default: break;
    }

    GE_CORE_ASSERT_MSG(false, "Unknown shader type: {}", static_cast<int>(type));
    return 0;
}

std::string loadShader(const std::string& filepath)
{
    GE_PROFILE_FUNC();

    std::ifstream fin(filepath, std::ios_base::binary);

    if (!fin.is_open()) {
        GE_CORE_ERR("Failed to open shader: '{}'\n", filepath);
        return {};
    }

    fin >> std::noskipws;
    return std::string{std::istreambuf_iterator<char>(fin),
                       std::istreambuf_iterator<char>()};
}

} // namespace

namespace GE::OpenGL {

Shader::Shader(Type type)
    : m_type{toGlType(type)}
{
    GE_PROFILE_FUNC();

    GLCall(m_id = glCreateShader(m_type));
}

Shader::~Shader()
{
    GE_PROFILE_FUNC();

    GLCall(glDeleteShader(m_id));
}

bool Shader::compileFromFile(const std::string& filepath)
{
    GE_PROFILE_FUNC();

    std::string source_code = loadShader(filepath);
    return !source_code.empty() ? compileFromSource(source_code) : false;
}

bool Shader::compileFromSource(const std::string& source_code)
{
    GE_PROFILE_FUNC();

    const GLchar* source = source_code.c_str();
    GLint status{GL_FALSE};

    GLCall(glShaderSource(m_id, 1, &source, nullptr));
    GLCall(glCompileShader(m_id));
    GLCall(glGetShaderiv(m_id, GL_COMPILE_STATUS, &status));

#ifndef GE_DEBUG
    if (status == GL_FALSE) {
        GLint msg_len{};
        GLCall(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &msg_len));

        std::vector<GLchar> msg(msg_len);
        GLCall(glGetShaderInfoLog(m_id, msg_len, nullptr, msg.data()));
        GE_CORE_ERR("Failed to compile shader: {}", msg.data());
    }
#endif // GE_DEBUG

    return status != GL_FALSE;
}

} // namespace GE::OpenGL
