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

#include "buffer_layout.h"

#include "ge/core/asserts.h"
#include "ge/debug/profile.h"

#define FLOAT_SIZE sizeof(float)
#define INT_SIZE   sizeof(int32_t)
#define BOOL_SIZE  1

namespace GE {

uint32_t BufferElement::getTypeSize(Type type)
{
    switch (type) {
        case GE_ELEMENT_FLOAT: return FLOAT_SIZE;
        case GE_ELEMENT_FLOAT2: return FLOAT_SIZE * 2;
        case GE_ELEMENT_FLOAT3: return FLOAT_SIZE * 3;
        case GE_ELEMENT_FLOAT4: return FLOAT_SIZE * 4;
        case GE_ELEMENT_MAT3: return FLOAT_SIZE * 3 * 3;
        case GE_ELEMENT_MAT4: return FLOAT_SIZE * 4 * 4;
        case GE_ELEMENT_INT: return INT_SIZE;
        case GE_ELEMENT_INT2: return INT_SIZE * 2;
        case GE_ELEMENT_INT3: return INT_SIZE * 3;
        case GE_ELEMENT_INT4: return INT_SIZE * 4;
        case GE_ELEMENT_BOOL: return BOOL_SIZE;
        default: break;
    }

    GE_CORE_ASSERT(false, "Unknown element type: {}", static_cast<int>(type));
    return 0;
}

uint32_t BufferElement::getComponentCount() const
{
    switch (m_type) {
        case GE_ELEMENT_FLOAT: return 1;
        case GE_ELEMENT_FLOAT2: return 2;
        case GE_ELEMENT_FLOAT3: return 3;
        case GE_ELEMENT_FLOAT4: return 4;
        case GE_ELEMENT_MAT3: return 3 * 3;
        case GE_ELEMENT_MAT4: return 4 * 4;
        case GE_ELEMENT_INT: return 1;
        case GE_ELEMENT_INT2: return 2;
        case GE_ELEMENT_INT3: return 3;
        case GE_ELEMENT_INT4: return 4;
        case GE_ELEMENT_BOOL: return 1;
        default: break;
    }

    GE_CORE_ASSERT(false, "Unknown element type: {}", static_cast<int>(m_type));
    return 0;
}

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_elements{elements}
{
    calculateOffsetsAndStride();
}

void BufferLayout::calculateOffsetsAndStride()
{
    GE_PROFILE_FUNC();
    m_stride = 0;

    for (auto& element : m_elements) {
        element.setOffset(m_stride);
        m_stride += element.size();
    }
}

} // namespace GE
