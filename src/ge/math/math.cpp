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

#include <ge/math/math.h>

#include <glm/gtx/matrix_decompose.hpp>

namespace GE::Math {

/**
 * Based on glm::decompose()
 * https://github.com/g-truc/glm/blob/0.9.9.8/glm/gtx/matrix_decompose.inl
 */
bool decomposeTransform(glm::mat4 transform, glm::vec3 *translation, glm::vec3 *rotation,
                        glm::vec3 *scale)
{
    static constexpr auto epsilon = glm::epsilon<float>();

    // Normalize the matrix
    if (glm::epsilonEqual(transform[3][3], 0.0f, epsilon)) {
        return false;
    }

    // Clear perspective partition
    if (glm::epsilonEqual(transform[0][3], 0.0f, epsilon) ||
        glm::epsilonEqual(transform[1][3], 0.0f, epsilon) ||
        glm::epsilonEqual(transform[2][3], 0.0f, epsilon)) {
        transform[0][3] = 0.0f;
        transform[1][3] = 0.0f;
        transform[2][3] = 0.0f;
        transform[3][3] = 1.0f;
    }

    // Translation
    *translation = glm::vec3{transform[3]};
    transform[3] = glm::vec4{0.0f, 0.0f, 0.0f, transform[3].w};

    // Scale
    glm::mat3 row = glm::mat3{transform};

    for (glm::mat3::length_type i{0}; i < glm::mat3::length(); i++) {
        (*scale)[i] = glm::length(row[i]);
        row[i] = glm::detail::scale(row[i], 1.0f);
    }

    // Rotation
    rotation->y = glm::asin(-row[0][2]);

    if (glm::cos(rotation->y) != 0.0f) {
        rotation->x = std::atan2(row[1][2], row[2][2]);
        rotation->z = std::atan2(row[0][1], row[0][0]);
    } else {
        rotation->x = std::atan2(-row[2][0], row[1][1]);
        rotation->z = 0.0f;
    }

    return true;
}

} // namespace GE::Math
