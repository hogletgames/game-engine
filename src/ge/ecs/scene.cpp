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

#include "scene.h"
#include "components.h"

#include "ge/core/begin.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"
#include "ge/renderer/renderer_2d.h"

namespace GE {

Scene::Scene()
    : m_registry{this}
{}

void Scene::onUpdate([[maybe_unused]] Timestamp delta_time)
{
    GE_PROFILE_FUNC();

    m_registry.onUpdate(delta_time);

    if (m_main_camera.isNull()) {
        return;
    }

    Begin<GE::Renderer2D> begin{m_main_camera};
    m_registry.drawEntities();
}

void Scene::onViewportResize(const glm::vec2& viewport)
{
    GE_PROFILE_FUNC();

    m_viewport = viewport;
    m_registry.onViewportResize(m_viewport);
}

Entity Scene::createEntity(const std::string& name)
{
    GE_PROFILE_FUNC();

    return m_registry.create(name);
}

Entity Scene::createCamera(const std::string& name)
{
    GE_PROFILE_FUNC();

    auto camera = createEntity(name);
    camera.addComponent<CameraComponent>();
    return camera;
}

void Scene::destroyEntity(const Entity& entity)
{
    GE_PROFILE_FUNC();

    m_registry.destroy(entity);
}

bool Scene::setMainCamera(const Entity& camera)
{
    GE_PROFILE_FUNC();

    if (!camera.hasComponent<CameraComponent>()) {
        GE_CORE_ERR("Trying to set entity without CameraComponent as main camera");
        return false;
    }

    m_main_camera = camera;
    return true;
}

} // namespace GE
