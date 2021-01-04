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

#include "camera_controller_script.h"
#include "components.h"

#include "ge/debug/profile.h"
#include "ge/window/input.h"

namespace GE {

CameraControllerScript::CameraControllerScript(const Entity& entity)
    : ScriptableEntity{entity, "Camera Controller"}
{
    GE_PROFILE_FUNC();
}

void CameraControllerScript::onUpdate(Timestamp dt)
{
    GE_PROFILE_FUNC();

    static constexpr float speed{5.0f};
    auto& translation = getComponent<TransformComponent>().translation;

    if (Input::isKeyPressed(KeyCode::LEFT)) {
        translation.x -= speed * dt;
    } else if (Input::isKeyPressed(KeyCode::RIGHT)) {
        translation.x += speed * dt;
    } else if (Input::isKeyPressed(KeyCode::DOWN)) {
        translation.y -= speed * dt;
    } else if (Input::isKeyPressed(KeyCode::UP)) {
        translation.y += speed * dt;
    }
}

} // namespace GE
