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

#include "ge/core/asserts.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace GE::OpenGL {

ShaderProgram::ShaderProgram(std::string name)
    : m_name(std::move(name))
{
    GE_PROFILE_FUNC();

    GLCall(m_id = glCreateProgram());
}

ShaderProgram::~ShaderProgram()
{
    GE_PROFILE_FUNC();

    GLCall(glDeleteProgram(m_id));
}

void ShaderProgram::addShader(Shared<Shader> shader)
{
    GE_PROFILE_FUNC();

    m_shaders.emplace_back(std::move(shader));
}

void ShaderProgram::addShaders(std::initializer_list<Shared<Shader>> shaders)
{
    GE_PROFILE_FUNC();

    std::move(shaders.begin(), shaders.end(), std::back_inserter(m_shaders));
}

bool ShaderProgram::link()
{
    GE_PROFILE_FUNC();

    GE_CORE_ASSERT_MSG(!m_shaders.empty(), "There are no shaders to link");
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
    return status != GL_FALSE;
}

void ShaderProgram::clear()
{
    GE_PROFILE_FUNC();

    m_shaders.clear();
}

void ShaderProgram::setUniformInt(const std::string& name, int value)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniform1i(location, value));
}

void ShaderProgram::setUniformIntArray(const std::string& name, const int* array,
                                       uint32_t count)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniform1iv(location, count, array));
}

void ShaderProgram::setUniformFloat(const std::string& name, float value)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniform1f(location, value));
}

void ShaderProgram::setUniformFloat2(const std::string& name, const glm::vec2& vector)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniform2f(location, vector.x, vector.y));
}

void ShaderProgram::setUniformFloat3(const std::string& name, const glm::vec3& vector)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniform3f(location, vector.x, vector.y, vector.z));
}

void ShaderProgram::setUniformFloat4(const std::string& name, const glm::vec4& vector)
{
    GE_PROFILE_FUNC();

    GLint location{0};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniform4f(location, vector.x, vector.y, vector.z, vector.w));
}

void ShaderProgram::setUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void ShaderProgram::setUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GE_PROFILE_FUNC();

    GLint location{-1};
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));
    GE_CORE_ASSERT_MSG(location != -1, "Failed to find uniform '{}'", name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void ShaderProgram::bind() const
{
    GE_PROFILE_FUNC();

    GLCall(glUseProgram(m_id));
}

void ShaderProgram::unbind() const
{
    GE_PROFILE_FUNC();

    GLCall(glUseProgram(0));
}

void ShaderProgram::attachShaders()
{
    GE_PROFILE_FUNC();

    for (const auto& shader : m_shaders) {
        GLCall(glAttachShader(m_id, shader->getNativeID()));
    }
}

void ShaderProgram::detachShaders()
{
    GE_PROFILE_FUNC();

    for (const auto& shader : m_shaders) {
        GLCall(glDetachShader(m_id, shader->getNativeID()));
    }
}

} // namespace GE::OpenGL
