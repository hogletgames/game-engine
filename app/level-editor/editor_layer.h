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

// NOLINTNEXTLINE(llvm-header-guard)
#ifndef GE_APP_LEVEL_EDITOR_EDITOR_LAYER_H_
#define GE_APP_LEVEL_EDITOR_EDITOR_LAYER_H_

#include <ge/empty_layer.h>
#include <ge/renderer/ortho_camera_controller.h>
#include <ge/renderer/renderer_2d.h>

namespace LE {

class GE_API EditorLayer: public GE::EmptyLayer
{
public:
    EditorLayer();

    void onAttach() override;
    void onUpdate(GE::Timestamp delta_time) override;
    void onEvent(GE::Event *event) override;
    void onGuiRender() override;

private:
    void showMenuBar();
    void showSettingsPanel();

    GE::OrthoCameraController m_camera_controller;
    GE::Renderer2D::quad_t m_editable_quad{};
};

} // namespace LE

#endif // GE_APP_LEVEL_EDITOR_EDITOR_LAYER_H_
