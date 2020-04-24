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

#ifndef GE_RENDERER_SHADER_PROGRAM_H_
#define GE_RENDERER_SHADER_PROGRAM_H_

#include <ge/core/non_copyable.h>
#include <ge/renderer/shader.h>

#include <glm/glm.hpp>

#include <memory>

namespace GE {

class GE_API ShaderProgram: public NonCopyable
{
public:
    virtual void addShader(Shared<Shader> shader) = 0;
    virtual void addShaders(std::initializer_list<Shared<Shader>> shaders) = 0;
    virtual bool link() = 0;
    virtual void clear() = 0;

    virtual void setUniformInt(const std::string& name, int value) = 0;
    virtual void setUniformIntArray(const std::string& name, const int* array,
                                    uint32_t count) = 0;
    virtual void setUniformFloat(const std::string& name, float value) = 0;
    virtual void setUniformFloat2(const std::string& name, const glm::vec2& vector) = 0;
    virtual void setUniformFloat3(const std::string& name, const glm::vec3& vector) = 0;
    virtual void setUniformFloat4(const std::string& name, const glm::vec4& vector) = 0;
    virtual void setUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual void setUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    static Scoped<ShaderProgram> create();
};

} // namespace GE

#endif // GE_RENDERER_SHADER_PROGRAM_H_
