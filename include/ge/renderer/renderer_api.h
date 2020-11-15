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

#ifndef GE_RENDERER_RENDERER_API_H_
#define GE_RENDERER_RENDERER_API_H_

#include <ge/renderer/vertex_array.h>

#include <glm/glm.hpp>

#include <iostream>
#include <memory>

#define GE_NONE_API    ::GE::RendererAPI::API::NONE
#define GE_OPEN_GL_API ::GE::RendererAPI::API::OPEN_GL

namespace GE {

class GE_API RendererAPI: public NonCopyable
{
public:
    enum class API : uint8_t
    {
        NONE = 0,
        OPEN_GL
    };

    struct capabilities_t {
        uint32_t max_texture_slots{};
    };

    explicit RendererAPI(API api)
        : m_api{api}
    {}

    virtual void clear(const glm::vec4& color) = 0;
    virtual void draw(const Shared<VertexArray>& vertex_array) = 0;
    virtual void draw(uint32_t index_count) = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    virtual const capabilities_t& getCapabilities() = 0;
    API getAPI() { return m_api; }

    static Scoped<RendererAPI> create(API api);

protected:
    Scoped<capabilities_t> m_capabilities;
    API m_api;
};

std::string toString(RendererAPI::API api);
RendererAPI::API toRendAPI(const std::string& api);

} // namespace GE

inline std::ostream& operator<<(std::ostream& os, ::GE::RendererAPI::API api)
{
    return os << ::GE::toString(api);
}

#endif // GE_RENDERER_RENDERER_API_H_
