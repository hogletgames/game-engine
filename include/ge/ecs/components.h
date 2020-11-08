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

#ifndef GE_ECS_COMPONENTS_H_
#define GE_ECS_COMPONENTS_H_

#include <ge/core/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

namespace GE {

struct GE_API SpriteRendererComponent {
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
};

struct GE_API TagComponent {
    std::string tag;
};

struct GE_API TransformComponent {
    glm::vec3 translation{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    glm::mat4 getTransform() const
    {
        glm::mat4 rot = glm::rotate(glm::mat4{1.0f}, rotation.x, {1, 0, 0});
        rot = glm::rotate(rot, rotation.y, {0, 1, 0});
        rot = glm::rotate(rot, rotation.z, {0, 0, 1});

        return glm::translate(glm::mat4{1.0f}, translation) * rot *
               glm::scale(glm::mat4{1.0f}, scale);
    }
};

} // namespace GE

#endif // GE_ECS_COMPONENTS_H_
