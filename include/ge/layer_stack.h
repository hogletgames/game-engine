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

#ifndef GE_LAYER_STACK_H_
#define GE_LAYER_STACK_H_

#include <ge/core/core.h>

#include <deque>
#include <memory>
#include <type_traits>

namespace GE {

class Layer;

class GE_API LayerStack
{
public:
    using Container = std::deque<Shared<Layer>>;
    using iterator = typename Container::iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_iterator = typename Container::const_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;

    ~LayerStack();

    void pushLayer(Shared<Layer> layer);
    void popLayer(const Shared<Layer>& layer);

    void pushOverlay(Shared<Layer> overlay);
    void popOverlay(const Shared<Layer>& overlay);

    iterator begin() { return m_stack.begin(); }
    iterator end() { return m_stack.end(); }
    const_iterator begin() const { return m_stack.begin(); }
    const_iterator end() const { return m_stack.end(); }

    reverse_iterator rbegin() { return m_stack.rbegin(); }
    reverse_iterator rend() { return m_stack.rend(); }
    const_reverse_iterator rbegin() const { return m_stack.rbegin(); }
    const_reverse_iterator rend() const { return m_stack.rend(); }

private:
    Container m_stack{};
    size_t m_last_layer_idx{0};
};

} // namespace GE

#endif // GE_LAYER_STACK_H_
