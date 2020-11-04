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

#include "sub_texture.h"

#include "ge/core/utils.h"

namespace GE {

SubTexture::SubTexture(Shared<Texture2D> texture, const glm::vec2& min,
                       const glm::vec2& max)
    : m_texture{std::move(texture)}
{
    m_tex_coords[0] = {min.x, min.y};
    m_tex_coords[1] = {max.x, min.y};
    m_tex_coords[2] = {max.x, max.y};
    m_tex_coords[3] = {min.x, max.y};
}

Scoped<SubTexture> SubTexture::create(Shared<Texture2D> texture, const glm::vec2& coords,
                                      const glm::vec2& sprite_size)
{
    glm::vec2 min = {(coords.x * sprite_size.x) / texture->getWidth(),
                     (coords.y * sprite_size.y) / texture->getHeight()};
    glm::vec2 max = {((coords.x + 1) * sprite_size.x) / texture->getWidth(),
                     ((coords.y + 1) * sprite_size.y) / texture->getHeight()};

    return makeScoped<SubTexture>(std::move(texture), min, max);
}

} // namespace GE