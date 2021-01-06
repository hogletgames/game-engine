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

#ifndef GE_ECS_ENTITY_REGISTRY_H_
#define GE_ECS_ENTITY_REGISTRY_H_

#include <ge/core/asserts.h>
#include <ge/core/core.h>
#include <ge/core/timestamp.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace GE {

class Entity;

class GE_API EntityRegistry
{
public:
    using ForeachCallback = std::function<void(Entity)>;

    Entity create();
    void destroy(const Entity& entity);

    void clear();

    template<typename T, typename... Args>
    T& addComponent(const Entity& entity, Args&&... args)
    {
        GE_ASSERT_MSG(!hasComponent<T>(entity), "Entity has already had this component");
        return m_registry.emplace<T>(getNativeID(entity), std::forward<Args>(args)...);
    }

    template<typename T>
    void removeComponent(const Entity& entity)
    {
        GE_ASSERT_MSG(hasComponent<T>(entity), "Unable to remove non-existent component");
        m_registry.remove<T>(getNativeID(entity));
    }

    template<typename T>
    T& getComponent(const Entity& entity)
    {
        GE_ASSERT_MSG(hasComponent<T>(entity), "Unable to get non-existent component");
        return m_registry.get<T>(getNativeID(entity));
    }

    template<typename T>
    bool hasComponent(const Entity& entity) const
    {
        return m_registry.has<T>(getNativeID(entity));
    }

    void eachEntity(const ForeachCallback& callback)
    {
        m_registry.each([&](auto entity_id) { callback({entity_id, this}); });
    }

    template<typename... Args>
    void eachEntityWith(const ForeachCallback& callback)
    {
        if constexpr (sizeof...(Args) > 1) {
            eachEntityGroup<Args...>(callback);
        } else {
            eachEntityView<Args...>(callback);
        }
    }

private:
    using NativeEntityID = entt::entity;

    template<typename Component, typename... Args>
    void eachEntityGroup(const ForeachCallback& callback)
    {
        auto group = m_registry.group<Component>(entt::get<Args...>);

        for (auto entity_id : group) {
            callback({entity_id, this});
        }
    }

    template<typename Component>
    void eachEntityView(const ForeachCallback& callback)
    {
        auto view = m_registry.view<Component>();

        for (auto entity_id : view) {
            callback({entity_id, this});
        }
    }

    static NativeEntityID getNativeID(const Entity& entity);

    entt::registry m_registry;
};

} // namespace GE

#endif // GE_ECS_ENTITY_REGISTRY_H_
