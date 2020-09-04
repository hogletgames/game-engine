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

#ifndef GE_RENDERER_SHADER_H_
#define GE_RENDERER_SHADER_H_

#include <ge/core/non_copyable.h>

#include <memory>

#define GE_NONE_SHADER     ::GE::Shader::NONE
#define GE_VERTEX_SHADER   ::GE::Shader::VERTEX
#define GE_FRAGMENT_SHADER ::GE::Shader::FRAGMENT

#define GE_VERT_EXT ".vert"
#define GE_FRAG_EXT ".frag"

namespace GE {

class GE_API Shader: public NonCopyable
{
public:
    enum Type : uint8_t
    {
        NONE = 0,
        VERTEX,
        FRAGMENT
    };

    virtual bool compileFromFile(const std::string& filepath) = 0;
    virtual bool compileFromSource(const std::string& source_code) = 0;

    virtual std::uint32_t getNativeID() const = 0;

    static Scoped<Shader> create(Type type);
};

} // namespace GE

#endif // GE_RENDERER_SHADER_H_
