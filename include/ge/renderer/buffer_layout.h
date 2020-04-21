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

#ifndef GE_RENDERER_BUFFER_LAYOUT_H_
#define GE_RENDERER_BUFFER_LAYOUT_H_

#include <ge/core/core.h>

#include <string>
#include <vector>

#define GE_ELEMENT_NONE   ::GE::BufferElement::Type::NONE
#define GE_ELEMENT_FLOAT  ::GE::BufferElement::Type::FLOAT
#define GE_ELEMENT_FLOAT2 ::GE::BufferElement::Type::FLOAT2
#define GE_ELEMENT_FLOAT3 ::GE::BufferElement::Type::FLOAT3
#define GE_ELEMENT_FLOAT4 ::GE::BufferElement::Type::FLOAT4
#define GE_ELEMENT_MAT3   ::GE::BufferElement::Type::MAT3
#define GE_ELEMENT_MAT4   ::GE::BufferElement::Type::MAT4
#define GE_ELEMENT_INT    ::GE::BufferElement::Type::INT
#define GE_ELEMENT_INT2   ::GE::BufferElement::Type::INT2
#define GE_ELEMENT_INT3   ::GE::BufferElement::Type::INT3
#define GE_ELEMENT_INT4   ::GE::BufferElement::Type::INT4
#define GE_ELEMENT_BOOL   ::GE::BufferElement::Type::BOOL

namespace GE {

class GE_API BufferElement
{
public:
    enum class Type : uint8_t
    {
        NONE = 0,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        MAT3,
        MAT4,
        INT,
        INT2,
        INT3,
        INT4,
        BOOL
    };

    BufferElement(Type type, std::string name, bool normalized = false)
        : m_type{type}
        , m_name{std::move(name)}
        , m_normalized{normalized}
        , m_size{getTypeSize(type)}
    {}

    static uint32_t getTypeSize(Type type);
    uint32_t getComponentCount() const;

    void setOffset(uint32_t offset) { m_offset = offset; }

    Type type() const { return m_type; }
    std::string name() const { return m_name; }
    bool normalized() const { return m_normalized; }
    uint32_t size() const { return m_size; }
    uint32_t offset() const { return m_offset; }

private:
    Type m_type{GE_ELEMENT_NONE};
    std::string m_name;
    bool m_normalized{false};
    uint32_t m_size{};
    uint32_t m_offset{};
};

class GE_API BufferLayout
{
public:
    using Elements = std::vector<BufferElement>;
    using iterator = typename Elements::iterator;
    using const_iterator = typename Elements::const_iterator;

    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    const Elements& getElements() const { return m_elements; }
    uint32_t getStride() const { return m_stride; }

    iterator begin() { return m_elements.begin(); }
    iterator end() { return m_elements.end(); }
    const_iterator begin() const { return m_elements.begin(); }
    const_iterator end() const { return m_elements.end(); }

private:
    void calculateOffsetsAndStride();

    Elements m_elements;
    uint32_t m_stride{};
};

} // namespace GE

#endif // GE_RENDERER_BUFFER_LAYOUT_H_
