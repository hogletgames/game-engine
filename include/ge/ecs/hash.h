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

#ifndef GE_ECS_HASH_H_
#define GE_ECS_HASH_H_

#include <ge/core/hash.h>
#include <ge/ecs/components.h>

namespace GE {

struct SceneCameraHash {
    size_t operator()(const SceneCamera::perspective_params_t& params) const
    {
        size_t seed{0};
        boost::hash_combine(seed, boost::hash_value(params.fov));
        boost::hash_combine(seed, boost::hash_value(params.near));
        boost::hash_combine(seed, boost::hash_value(params.far));
        return seed;
    }

    size_t operator()(const SceneCamera::orthographic_params_t& params) const
    {
        size_t seed{0};
        boost::hash_combine(seed, boost::hash_value(params.size));
        boost::hash_combine(seed, boost::hash_value(params.near));
        boost::hash_combine(seed, boost::hash_value(params.far));
        return seed;
    }

    size_t operator()(const SceneCamera& camera) const
    {
        size_t seed{0};
        boost::hash_combine(seed, boost::hash_value(camera.getProjectionType()));
        boost::hash_combine(seed, MathTypesHash{}(camera.getProjection()));
        boost::hash_combine(seed, operator()(camera.getPerspective()));
        boost::hash_combine(seed, operator()(camera.getOrthographic()));
        return seed;
    }
};

struct ComponentHash {
    size_t operator()(const CameraComponent& camera) const
    {
        size_t seed{0};
        boost::hash_combine(seed, SceneCameraHash{}(camera.camera));
        boost::hash_combine(seed, boost::hash_value(camera.fixed_aspect_ratio));
        return seed;
    }

    size_t operator()(const NativeScriptComponent& script) const
    {
        size_t seed{0};
        boost::hash_combine(seed, boost::hash_value(script.getScript()));
        return seed;
    }

    size_t operator()(const SpriteRendererComponent& sprite_renderer) const
    {
        size_t seed{0};
        boost::hash_combine(seed, MathTypesHash{}(sprite_renderer.color));
        return seed;
    }

    size_t operator()(const TagComponent& tag) const
    {
        size_t seed{0};
        boost::hash_combine(seed, boost::hash_value(tag.tag));
        return seed;
    }

    size_t operator()(const TransformComponent& transform) const
    {
        size_t seed{0};
        boost::hash_combine(seed, MathTypesHash{}(transform.getTransform()));
        return seed;
    }
};

} // namespace GE

#endif // GE_ECS_HASH_H_
