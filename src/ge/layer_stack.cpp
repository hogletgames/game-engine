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

#include "layer_stack.h"
#include "layer.h"

#include <algorithm>

namespace GE {

LayerStack::LayerStack(LayerStack&& other) noexcept(
    std::is_nothrow_move_assignable<Container>::value)
{
    *this = std::move(other);
}

LayerStack& LayerStack::operator=(LayerStack&& other) noexcept(
    std::is_nothrow_move_assignable<Container>::value)
{
    if (this == &other) {
        return *this;
    }

    m_stack = std::move(other.m_stack);
    m_last_layer_idx = std::exchange(other.m_last_layer_idx, 0);
    return *this;
}

LayerStack::~LayerStack()
{
    for (auto& layer : m_stack) {
        layer->onDetach();
    }
}

void LayerStack::pushLayer(Shared<Layer> layer)
{
    m_stack.emplace(std::next(m_stack.begin(), m_last_layer_idx), std::move(layer));
    m_last_layer_idx++;
}

void LayerStack::popLayer(const Shared<Layer>& layer)
{
    auto layer_end = std::next(m_stack.begin(), m_last_layer_idx);
    auto layer_in_stack = std::find(m_stack.begin(), layer_end, layer);

    if (layer_in_stack != layer_end) {
        (*layer_in_stack)->onDetach();
        m_stack.erase(layer_in_stack);
        m_last_layer_idx--;
    }
}

void LayerStack::pushOverlay(Shared<Layer> overlay)
{
    m_stack.emplace_back(std::move(overlay));
}

void LayerStack::popOverlay(const Shared<Layer>& overlay)
{
    auto overlay_begin = std::next(m_stack.begin(), m_last_layer_idx);
    auto overlay_in_stack = std::find(overlay_begin, m_stack.end(), overlay);

    if (overlay_in_stack != m_stack.end()) {
        (*overlay_in_stack)->onDetach();
        m_stack.erase(overlay_in_stack);
    }
}

} // namespace GE
