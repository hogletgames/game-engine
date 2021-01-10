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

#ifndef GE_ECS_SCENE_H_
#define GE_ECS_SCENE_H_

#include <ge/core/timestamp.h>
#include <ge/core/type_list.h>
#include <ge/ecs/components.h>
#include <ge/ecs/entity.h>
#include <ge/ecs/entity_registry.h>

#include <glm/glm.hpp>

#include <string>

namespace GE {

class Entity;

class GE_API Scene
{
public:
    using ForeachCallback = EntityRegistry::ForeachCallback;
    using DefaultComponents = TypeList<TagComponent, TransformComponent>;

    void onViewportResize(const glm::vec2& viewport);

    void updateEntities(Timestamp dt);
    void drawEntities();

    void eachEntity(const ForeachCallback& callback);

    Entity createEntity(const std::string& name = {});
    Entity createCamera(const std::string& name = {});
    void destroyEntity(const Entity& entity);

    const Entity& getMainCamera() const { return m_main_camera; }
    bool setMainCamera(const Entity& camera);

private:
    EntityRegistry m_registry;
    Entity m_main_camera;

    glm::vec2 m_viewport{0.0f, 0.0f};
};

} // namespace GE

#endif // GE_ECS_SCENE_H_
