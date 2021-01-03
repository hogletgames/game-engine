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

#ifndef GE_ECS_ENTITY_H_
#define GE_ECS_ENTITY_H_

#include <ge/ecs/entity_registry.h>

#include <entt/entt.hpp>

namespace GE {

class GE_API Entity
{
public:
    using ID = entt::entity;

    Entity() = default;
    Entity(ID id, EntityRegistry* registry);

    ID getID() const { return m_id; }
    bool isNull() const { return m_id == nullID(); }

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        return m_registry->addComponent<T>(*this, std::forward<Args>(args)...);
    }

    template<typename T>
    void removeComponent()
    {
        m_registry->removeComponent<T>(*this);
    }

    template<typename T>
    const T& getComponent() const
    {
        return m_registry->getComponent<T>(*this);
    }

    template<typename T>
    T& getComponent()
    {
        return m_registry->getComponent<T>(*this);
    }

    template<typename T>
    bool hasComponent() const
    {
        return m_registry->hasComponent<T>(*this);
    }

    static constexpr ID nullID() { return entt::null; }

    bool operator==(const Entity& other) const
    {
        return m_id == other.m_id && m_registry == other.m_registry;
    }

    bool operator!=(const Entity& other) const { return !operator==(other); }

private:
    ID m_id{nullID()};
    EntityRegistry* m_registry{nullptr};
};

} // namespace GE

#endif // GE_ECS_ENTITY_H_
