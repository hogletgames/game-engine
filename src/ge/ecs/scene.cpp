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

namespace {

constexpr auto ENTITY_TAG_DEFAULT = "Entity";

} // namespace

namespace GE {

Scene::Scene(std::string name)
    : m_name{std::move(name)}
{
    GE_PROFILE_FUNC();
}

void Scene::onUpdate(Timestamp dt)
{
    GE_PROFILE_FUNC();

    m_registry.eachEntityWith<NativeScriptComponent>([dt](Entity entity) {
        entity.getComponent<NativeScriptComponent>().onUpdate(dt);
    });

    if (m_main_camera.isNull()) {
        return;
    }

    Begin<GE::Renderer2D> begin{m_main_camera};

    m_registry.eachEntityWith<TransformComponent, SpriteRendererComponent>(
        [](Entity entity) { Renderer2D::draw(entity); });
}

void Scene::onViewportResize(const glm::vec2& viewport)
{
    GE_PROFILE_FUNC();

    m_viewport = viewport;

    m_registry.eachEntityWith<CameraComponent>([this](Entity entity) {
        auto& camera = entity.getComponent<CameraComponent>();

        if (!camera.fixed_aspect_ratio) {
            camera.camera.setViewport(m_viewport);
        }
    });
}

void Scene::eachEntity(const ForeachCallback& callback)
{
    GE_PROFILE_FUNC();

    m_registry.eachEntity(callback);
}

Entity Scene::createEntity(const std::string& name)
{
    GE_PROFILE_FUNC();

    Entity entity = m_registry.create();
    entity.addComponent<TransformComponent>();
    auto& tag = entity.addComponent<TagComponent>().tag;
    tag = name.empty() ? ENTITY_TAG_DEFAULT : name;

    return entity;
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

    if (entity == m_main_camera) {
        setMainCamera({});
    }

    m_registry.destroy(entity);
}

bool Scene::setMainCamera(const Entity& camera)
{
    GE_PROFILE_FUNC();

    if (!camera.isNull() && !camera.hasComponent<CameraComponent>()) {
        GE_CORE_ERR("Trying to set entity without CameraComponent as main camera");
        return false;
    }

    m_main_camera = camera;
    return true;
}

} // namespace GE
