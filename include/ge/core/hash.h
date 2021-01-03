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

#ifndef GE_CORE_HASH_H_
#define GE_CORE_HASH_H_

#include <boost/functional/hash.hpp>
#include <glm/glm.hpp>

namespace GE {

struct MathTypesHash {
    size_t operator()(const glm::vec3& vec) const { return getVectorHash(vec); }

    size_t operator()(const glm::vec4& vec) const { return getVectorHash(vec); }

    size_t operator()(const glm::mat4& mat) const { return getMatHash(mat); }

private:
    template<typename Vec>
    size_t getVectorHash(const Vec& vec) const
    {
        size_t seed{0};

        for (int i{0}; i < vec.length(); i++) {
            boost::hash_combine(seed, boost::hash_value(vec[i]));
        }

        return seed;
    }

    template<typename Mat>
    size_t getMatHash(const Mat& mat) const
    {
        size_t seed{0};

        for (int i{0}; i < mat.length(); i++) {
            for (int j{0}; j < mat[i].length(); j++) {
                boost::hash_combine(seed, boost::hash_value(mat[i][j]));
            }
        }

        return seed;
    }
};

} // namespace GE

#endif // GE_CORE_HASH_H_
