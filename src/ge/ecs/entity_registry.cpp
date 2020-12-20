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

#include "entity_registry.h"
#include "components.h"
#include "entity.h"

#include "ge/debug/profile.h"

namespace {

constexpr auto ENTITY_TAG_DEFAULT = "Entity";

} // namespace

namespace GE {

EntityRegistry::EntityRegistry(Scene* scene)
    : m_scene{scene}
{
    GE_PROFILE_FUNC();

    GE_UNUSED(m_scene);
}

Entity EntityRegistry::create(const std::string& name)
{
    GE_PROFILE_FUNC();

    Entity entity{m_registry.create(), this};

    entity.addComponent<TransformComponent>();

    auto& tag = entity.addComponent<TagComponent>().tag;
    tag = name.empty() ? ENTITY_TAG_DEFAULT : name;

    return entity;
}

void EntityRegistry::destroy(const Entity& entity)
{
    GE_PROFILE_FUNC();

    m_registry.destroy(entity.getID());
}

EntityRegistry::NativeEntityID EntityRegistry::getNativeID(const Entity& entity)
{
    GE_PROFILE_FUNC();

    return entity.getID();
}

} // namespace GE
