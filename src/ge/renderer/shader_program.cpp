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
#include "opengl/shader_program.h"

#include "ge/core/asserts.h"
#include "ge/core/log.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"
#include "ge/renderer/renderer.h"

#include <filesystem>

namespace {

bool compileShader(const GE::Shared<GE::Shader>& shader, const std::string& path)
{
    GE_PROFILE_FUNC();

    if (!shader->compileFromFile(path)) {
        GE_CORE_ERR("Failed to compile '{}'", path);
        return false;
    }

    return true;
}

} // namespace

namespace GE {

Scoped<ShaderProgram> ShaderProgram::create(std::string name)
{
    GE_PROFILE_FUNC();

    switch (Renderer::getAPI()) {
        case GE_OPEN_GL_API: return makeScoped<OpenGL::ShaderProgram>(std::move(name));
        default: GE_CORE_ASSERT_MSG(false, "Unsupported API: '{}'", Renderer::getAPI());
    }

    return nullptr;
}

bool ShaderLibrary::add(Shared<ShaderProgram> shader_program)
{
    GE_PROFILE_FUNC();

    std::string name = shader_program->getName();
    return add(std::move(shader_program), name);
}

bool ShaderLibrary::add(Shared<ShaderProgram> shader_program, std::string name)
{
    GE_PROFILE_FUNC();

    if (exists(name)) {
        GE_CORE_ERR("Shader '{}' already exists", name);
        return false;
    }

    m_shaders.emplace(std::move(name), std::move(shader_program));
    return true;
}

Shared<ShaderProgram> ShaderLibrary::load(const std::string& vertex_path,
                                          const std::string& fragment_path)
{
    GE_PROFILE_FUNC();

    std::string name = std::filesystem::path(vertex_path).stem();
    return load(vertex_path, fragment_path, name);
}

Shared<ShaderProgram> ShaderLibrary::load(const std::string& vertex_path,
                                          const std::string& fragment_path,
                                          const std::string& name)
{
    GE_PROFILE_FUNC();

    if (exists(name)) {
        GE_CORE_ERR("Shader '{}' already exists", name);
        return nullptr;
    }

    Shared<ShaderProgram> shader_program = ShaderProgram::create(name);
    Shared<Shader> vertex = Shader::create(GE_VERTEX_SHADER);
    Shared<Shader> fragment = Shader::create(GE_FRAGMENT_SHADER);

    if (!compileShader(vertex, vertex_path) || !compileShader(fragment, fragment_path)) {
        return nullptr;
    }

    shader_program->addShaders({fragment, vertex});

    if (!shader_program->link()) {
        GE_CORE_ERR("Failed to link '{}'", shader_program->getName());
        return nullptr;
    }

    if (!add(shader_program, name)) {
        GE_CORE_ERR("Failed to add '{}'", shader_program->getName());
        return nullptr;
    }

    return shader_program;
}

void ShaderLibrary::clear()
{
    m_shaders.clear();
}

Shared<ShaderProgram> ShaderLibrary::get(const std::string& name)
{
    GE_PROFILE_FUNC();

    if (!exists(name)) {
        GE_CORE_ERR("Shader '{}' doesn't exists", name);
        return nullptr;
    }

    return m_shaders[name];
}

bool ShaderLibrary::exists(const std::string& name) const
{
    return m_shaders.find(name) != m_shaders.end();
}

} // namespace GE
