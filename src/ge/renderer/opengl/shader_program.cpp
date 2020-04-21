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

#include "shader_program.h"
#include "opengl_utils.h"

#include <glad/glad.h>

namespace GE::OpenGL {

ShaderProgram::ShaderProgram()
{
    GLCall(m_id = glCreateProgram());
}

ShaderProgram::~ShaderProgram()
{
    GLCall(glDeleteProgram(m_id));
}

void ShaderProgram::addShader(Shared<Shader> shader)
{
    m_shaders.emplace_back(std::move(shader));
}

void ShaderProgram::addShaders(std::initializer_list<Shared<Shader>> shaders)
{
    std::move(shaders.begin(), shaders.end(), std::back_inserter(m_shaders));
}

bool ShaderProgram::link()
{
    GE_CORE_ASSERT(!m_shaders.empty(), "There are no shaders to link");
    GLint status{GL_FALSE};

    attachShaders();
    GLCall(glLinkProgram(m_id));
    GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &status));

    if (status == GL_FALSE) {
        GLint msg_len{};
        GLCall(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &msg_len));

        std::vector<GLchar> msg(msg_len);
        GLCall(glGetProgramInfoLog(m_id, msg_len, nullptr, msg.data()));
        GE_CORE_ERR("Failed to link shader program: {}", msg.data());
    }

    detachShaders();
    clear();
    return status == GL_FALSE;
}

void ShaderProgram::clear()
{
    m_shaders.clear();
}

void ShaderProgram::bind() const
{
    GLCall(glUseProgram(m_id));
}

void ShaderProgram::unbind() const
{
    GLCall(glUseProgram(0));
}

void ShaderProgram::attachShaders()
{
    for (const auto& shader : m_shaders) {
        GLCall(glAttachShader(m_id, shader->getNativeID()));
    }
}

void ShaderProgram::detachShaders()
{
    for (const auto& shader : m_shaders) {
        GLCall(glDetachShader(m_id, shader->getNativeID()));
    }
}

} // namespace GE::OpenGL
