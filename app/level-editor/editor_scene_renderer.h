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
#ifndef LE_EDITOR_SCENE_RENDERER_H_
#define LE_EDITOR_SCENE_RENDERER_H_

#include <ge/renderer/scene_renderer_base.h>

namespace GE {

class VPCameraController;

} // namespace GE

namespace LE {

class GE_API EditorSceneRenderer: public GE::SceneRendererBase
{
public:
    explicit EditorSceneRenderer(GE::Shared<GE::VPCameraController> camera_controller,
                                 GE::Shared<GE::Scene> scene = nullptr);

    void onUpdate(GE::Timestamp dt) override;

    void setScene(GE::Shared<GE::Scene> scene) override { m_scene = std::move(scene); }
    const GE::Shared<GE::Scene>& getScene() const override { return m_scene; }

private:
    GE::Shared<GE::VPCameraController> m_camera_controller;
    GE::Shared<GE::Scene> m_scene;
};

} // namespace LE

#endif // LE_EDITOR_SCENE_RENDERER_H_
