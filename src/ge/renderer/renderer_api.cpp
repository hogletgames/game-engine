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

#include "renderer_api.h"
#include "opengl/renderer_api.h"

#include "ge/core/asserts.h"
#include "ge/core/log.h"
#include "ge/core/utils.h"

#define API_NONE_STR    "None"
#define API_OPEN_GL_STR "OpenGL"

namespace GE {

Scoped<RendererAPI> RendererAPI::create(API api)
{
    GE_CORE_DBG("New Renderer API: {}", api);

    switch (api) {
        case GE_OPEN_GL_API: return makeScoped<OpenGL::RendererAPI>(api);
        default: GE_CORE_ASSERT_MSG(false, "Unsupported API: '{}'", api);
    }

    return nullptr;
}

std::string toString(RendererAPI::API api)
{
    std::unordered_map<RendererAPI::API, std::string> api_to_str{
        {GE_NONE_API, API_NONE_STR}, {GE_OPEN_GL_API, API_OPEN_GL_STR}};

    return toType(api_to_str, api, {});
}

RendererAPI::API toRendAPI(const std::string& api)
{
    std::unordered_map<std::string, RendererAPI::API> str_to_api{
        {API_NONE_STR, GE_NONE_API}, {API_OPEN_GL_STR, GE_OPEN_GL_API}};

    return toType(str_to_api, api, GE_NONE_API);
}

} // namespace GE
