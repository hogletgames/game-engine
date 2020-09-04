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
#ifndef GE_RENDERER_OPENGL_SHADER_PROGRAM_H_
#define GE_RENDERER_OPENGL_SHADER_PROGRAM_H_

#include "ge/renderer/shader_program.h"

#include <vector>

namespace GE::OpenGL {

class ShaderProgram: public ::GE::ShaderProgram
{
public:
    explicit ShaderProgram(std::string name);
    ~ShaderProgram() override;

    void addShader(Shared<Shader> shader) override;
    void addShaders(std::initializer_list<Shared<Shader>> shaders) override;
    bool link() override;
    void clear() override;

    void setUniformInt(const std::string& name, int value) override;
    void setUniformIntArray(const std::string& name, const int* array,
                            uint32_t count) override;
    void setUniformFloat(const std::string& name, float value) override;
    void setUniformFloat2(const std::string& name, const glm::vec2& vector) override;
    void setUniformFloat3(const std::string& name, const glm::vec3& vector) override;
    void setUniformFloat4(const std::string& name, const glm::vec4& vector) override;
    void setUniformMat3(const std::string& name, const glm::mat3& matrix) override;
    void setUniformMat4(const std::string& name, const glm::mat4& matrix) override;

    void bind() const override;
    void unbind() const override;

    const std::string& getName() const override { return m_name; }

private:
    void attachShaders();
    void detachShaders();

    uint32_t m_id{0};
    std::string m_name;
    std::vector<Shared<Shader>> m_shaders;
};

} // namespace GE::OpenGL

#endif // GE_RENDERER_OPENGL_SHADER_PROGRAM_H_
