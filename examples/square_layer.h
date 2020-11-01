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

// NOLINTNEXTLINE
#ifndef GE_EXAMPLES_SQUARE_LAYER_H_
#define GE_EXAMPLES_SQUARE_LAYER_H_

#include "gui_layer.h"

namespace GE::Examples {

class GE_API SquareLayer: public GuiLayer
{
public:
    explicit SquareLayer(bool show_gui_demo);

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Timestamp delta_time) override;
    void onEvent(Event* event) override;
    void onGuiRender() override;

private:
    enum SquareType : uint8_t
    {
        COLORED = 0,
        TEXTURED
    };

    Shared<ShaderProgram> bindCurrentShader();

    ShaderLibrary m_shader_library;
    Shared<VertexArray> m_vao;
    Shared<Texture2D> m_texture;

    int m_square_type{};
    glm::vec4 m_square_color{};
};

} // namespace GE::Examples

#endif // GAME_ENGINE_SQUARE_LAYER_H_