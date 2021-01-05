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

#include "scene_serializer.h"
#include "camera_controller_script.h"
#include "scene.h"

#include "ge/core/log.h"
#include "ge/debug/profile.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace {

constexpr uint32_t MAGIC_WORD{0xAD9A98EB};
constexpr uint32_t VERSION{0x00010000};

} // namespace

namespace YAML {
/**
 * Third-party types
 */
template<>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& vec)
    {
        Node node;
        node.push_back(vec.x);
        node.push_back(vec.y);
        node.push_back(vec.z);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    // NOLINTNEXTLINE(google-runtime-references)
    static bool decode(const Node& node, glm::vec3& vec)
    {
        if (!node.IsSequence() || node.size() != 3) {
            return false;
        }

        vec.x = node[0].as<float>();
        vec.y = node[1].as<float>();
        vec.z = node[2].as<float>();
        return true;
    }
};

template<>
struct convert<glm::vec4> {
    static Node encode(const glm::vec4& vec)
    {
        Node node;
        node.push_back(vec.x);
        node.push_back(vec.y);
        node.push_back(vec.z);
        node.push_back(vec.w);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    // NOLINTNEXTLINE(google-runtime-references)
    static bool decode(const Node& node, glm::vec4& vec)
    {
        if (!node.IsSequence() || node.size() != 4) {
            return false;
        }

        vec.x = node[0].as<float>();
        vec.y = node[1].as<float>();
        vec.z = node[2].as<float>();
        vec.w = node[3].as<float>();
        return true;
    }
};

/**
 * User-Defined types
 */
template<>
struct convert<GE::Entity::ID> {
    static Node encode(const GE::Entity::ID& entity_id)
    {
        Node node;
        node.push_back(static_cast<uint32_t>(entity_id));
        return node;
    }

    // NOLINTNEXTLINE(google-runtime-references)
    static bool decode(const Node& node, GE::Entity::ID& entity_id)
    {
        entity_id = static_cast<GE::Entity::ID>(node.as<uint32_t>());
        return true;
    }
};

template<>
struct convert<GE::SceneCamera::perspective_params_t> {
    using Perspective = GE::SceneCamera::perspective_params_t;

    static Node encode(const Perspective& perspective)
    {
        YAML::Node node;
        node.push_back(perspective.fov);
        node.push_back(perspective.near);
        node.push_back(perspective.far);
        return node;
    }

    // NOLINTNEXTLINE(google-runtime-references)
    static bool decode(const Node& node, Perspective& perspective)
    {
        if (!node.IsMap() || node.size() != 3) {
            return false;
        }

        perspective.fov = node["Vertical FOV"].as<float>();
        perspective.near = node["Near"].as<float>();
        perspective.far = node["Far"].as<float>();
        return true;
    }
};

template<>
struct convert<GE::SceneCamera::orthographic_params_t> {
    using Orthographic = GE::SceneCamera::orthographic_params_t;

    static Node encode(const Orthographic& orthographic)
    {
        YAML::Node node;
        node.push_back(orthographic.size);
        node.push_back(orthographic.near);
        node.push_back(orthographic.far);
        return node;
    }

    // NOLINTNEXTLINE(google-runtime-references)
    static bool decode(const Node& node, Orthographic& orthographic)
    {
        if (!node.IsMap() || node.size() != 3) {
            return false;
        }

        orthographic.size = node["Size"].as<float>();
        orthographic.near = node["Near"].as<float>();
        orthographic.far = node["Far"].as<float>();
        return true;
    }
};

template<>
struct convert<GE::SceneCamera::ProjectionType> {
    static Node encode(const GE::SceneCamera::ProjectionType& projection_type)
    {
        YAML::Node node;
        node.push_back(GE::toString(projection_type));
        return node;
    }

    // NOLINTNEXTLINE(google-runtime-references)
    static bool decode(const Node& node, GE::SceneCamera::ProjectionType& projection_type)
    {
        projection_type = GE::toCameraProjection(node.as<std::string>());
        return true;
    }
};

} // namespace YAML

namespace {

/**
 * Serialization
 */
YAML::Emitter& operator<<(YAML::Emitter& yaml, const glm::vec3& vec)
{
    yaml << YAML::Flow;
    yaml << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
    return yaml;
}

YAML::Emitter& operator<<(YAML::Emitter& yaml, const glm::vec4& vec)
{
    yaml << YAML::Flow;
    yaml << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
    return yaml;
}

void serializeComponent(YAML::Emitter* yaml, const GE::TagComponent& component)
{
    *yaml << YAML::Key << GE::TagComponent::name();
    *yaml << YAML::BeginMap;
    *yaml << YAML::Key << "Tag" << YAML::Value << component.tag;
    *yaml << YAML::EndMap;
}

void serializeComponent(YAML::Emitter* yaml,
                        [[maybe_unused]] const GE::TransformComponent& component)
{
    *yaml << YAML::Key << GE::TransformComponent::name();
    *yaml << YAML::BeginMap;
    *yaml << YAML::Key << "Translation" << YAML::Value << component.translation;
    *yaml << YAML::Key << "Rotation" << YAML::Value << component.rotation;
    *yaml << YAML::Key << "Scale" << YAML::Value << component.scale;
    *yaml << YAML::EndMap;
}

void serializeComponent(YAML::Emitter* yaml, const GE::CameraComponent& component)
{
    std::string projection_type = GE::toString(component.camera.getProjectionType());
    const auto& perspective = component.camera.getPerspective();
    const auto& orthographic = component.camera.getOrthographic();

    *yaml << YAML::Key << GE::CameraComponent::name();
    *yaml << YAML::BeginMap; // Camera

    *yaml << YAML::Key << "Fixed Aspect Ratio" << YAML::Value
          << component.fixed_aspect_ratio;

    *yaml << YAML::Key << "Scene Camera";
    *yaml << YAML::BeginMap; // Scene Camera
    *yaml << YAML::Key << "Projection Type" << YAML::Value << projection_type;

    *yaml << YAML::Key << "Perspective";
    *yaml << YAML::Value << YAML::BeginMap;
    *yaml << YAML::Key << "Vertical FOV" << YAML::Value << perspective.fov;
    *yaml << YAML::Key << "Near" << YAML::Value << perspective.near;
    *yaml << YAML::Key << "Far" << YAML::Value << perspective.far;
    *yaml << YAML::EndMap;

    *yaml << YAML::Key << "Orthographic";
    *yaml << YAML::BeginMap;
    *yaml << YAML::Key << "Size" << YAML::Value << orthographic.size;
    *yaml << YAML::Key << "Near" << YAML::Value << orthographic.near;
    *yaml << YAML::Key << "Far" << YAML::Value << orthographic.far;
    *yaml << YAML::EndMap;

    *yaml << YAML::EndMap; // Scene Camera
    *yaml << YAML::EndMap; // Camera
}

void serializeComponent(YAML::Emitter* yaml, const GE::SpriteRendererComponent& component)
{
    *yaml << YAML::Key << GE::SpriteRendererComponent::name();
    *yaml << YAML::BeginMap;
    *yaml << YAML::Key << "Color" << YAML::Value << component.color;
    *yaml << YAML::EndMap;
}

void serializeComponent(YAML::Emitter* yaml, const GE::NativeScriptComponent& component)
{
    *yaml << YAML::Key << GE::NativeScriptComponent::name();
    *yaml << YAML::BeginMap;
    *yaml << YAML::Key << "Script" << YAML::Value << component.scriptName();
    *yaml << YAML::EndMap;
}

void serializeEntity(YAML::Emitter* yaml, const GE::Entity& entity)
{
    *yaml << YAML::BeginMap;
    *yaml << YAML::Key << "Entity" << GE::toString(entity.getID());

    GE::forEachType<GE::ComponentsList>([yaml, &entity](auto component) {
        using Component = decltype(component);
        if (entity.hasComponent<Component>()) {
            serializeComponent(yaml, entity.getComponent<Component>());
        }
    });

    *yaml << YAML::EndMap;
}

/**
 * Deserialization
 */
template<typename T>
T getYamlValue(const YAML::Node& node, const std::string& key, T def_value = T())
{
    try {
        return node[key].as<T>();
    } catch (const std::exception& e) {
        GE_CORE_ERR("Failed to get '{}': '{}'", key, e.what());
        return def_value;
    }
}

template<typename Component>
void deserializeComponent(const YAML::Node& yaml, GE::Entity* entity);

template<>
void deserializeComponent<GE::TagComponent>(const YAML::Node& yaml, GE::Entity* entity)
{
    auto& tag = entity->getComponent<GE::TagComponent>();
    tag.tag = getYamlValue<std::string>(yaml, "Tag");
}

template<>
void deserializeComponent<GE::TransformComponent>(const YAML::Node& yaml,
                                                  GE::Entity* entity)
{
    auto& t = entity->getComponent<GE::TransformComponent>();
    t.translation = getYamlValue<glm::vec3>(yaml, "Translation", {0.0f, 0.0f, 0.0f});
    t.rotation = getYamlValue<glm::vec3>(yaml, "Rotation", {0.0f, 0.0f, 0.0f});
    t.scale = getYamlValue<glm::vec3>(yaml, "Scale", {1.0f, 1.0f, 1.0f});
}

template<>
void deserializeComponent<GE::CameraComponent>(const YAML::Node& yaml, GE::Entity* entity)
{
    using ProjectionType = GE::SceneCamera::ProjectionType;
    using Persective = GE::SceneCamera::perspective_params_t;
    using Orthographic = GE::SceneCamera::orthographic_params_t;

    auto& camera = entity->getComponent<GE::CameraComponent>();
    camera.fixed_aspect_ratio = getYamlValue<bool>(yaml, "Fixed Aspect Ratio");

    auto scene_camera = yaml["Scene Camera"];

    if (!scene_camera) {
        GE_CORE_WARN("Failed to load Scene Camera");
        return;
    }

    auto perspective = getYamlValue<Persective>(scene_camera, "Perspective");
    auto orthographic = getYamlValue<Orthographic>(scene_camera, "Orthographic");
    auto projection_type = getYamlValue<ProjectionType>(scene_camera, "Projection Type");

    camera.camera.setPerspective(perspective);
    camera.camera.setOrthographic(orthographic);
    camera.camera.setProjectionType(projection_type);
}

template<>
void deserializeComponent<GE::SpriteRendererComponent>(const YAML::Node& yaml,
                                                       GE::Entity* entity)
{
    auto& s = entity->getComponent<GE::SpriteRendererComponent>();
    s.color = getYamlValue<glm::vec4>(yaml, "Color", {0.0f, 0.0f, 0.0f, 0.0f});
}

template<>
void deserializeComponent<GE::NativeScriptComponent>(const YAML::Node& yaml,
                                                     GE::Entity* entity)
{
    auto& script = entity->getComponent<GE::NativeScriptComponent>();
    auto script_name = getYamlValue<std::string>(yaml, "Script");

    if (script_name == "Camera Controller") {
        script.bind<GE::CameraControllerScript>(*entity);
    } else {
        GE_CORE_WARN("Unsupported Native Script: '{}'", script_name);
    }
}

void deserializeEntity(const YAML::Node& yaml, GE::Scene* scene)
{
    auto entity_id = getYamlValue<GE::Entity::ID>(yaml, "Entity", GE::Entity::nullID());

    if (entity_id == GE::Entity::nullID()) {
        GE_CORE_WARN("Entity with NULL ID has been deserialized, skipping...");
        return;
    }

    GE::Entity entity = scene->createEntity(entity_id);

    GE::forEachType<GE::ComponentsList>([&yaml, &entity](auto component) {
        using Component = decltype(component);
        auto serialized_component = yaml[Component::name()];

        if (!serialized_component) {
            return;
        }

        if constexpr (!GE::isListContains<GE::Scene::DefaultComponents, Component>()) {
            entity.addComponent<Component>();
        }

        deserializeComponent<Component>(serialized_component, &entity);
    });
}

} // namespace

namespace GE {

bool SceneSerializer::serialize(const std::string& filepath, Scene* scene)
{
    GE_PROFILE_FUNC();

    std::ofstream fout(filepath);
    YAML::Emitter yaml;

    if (!fout) {
        GE_CORE_ERR("Failed to out Scene Serialization file: {}", filepath);
        return false;
    }

    yaml << YAML::BeginMap; // Scene
    yaml << YAML::Key << "Magic Word" << YAML::Value << YAML::Hex << MAGIC_WORD;
    yaml << YAML::Key << "Protocol Version" << YAML::Value << YAML::Hex << VERSION;
    yaml << YAML::Key << "Scene" << YAML::Value << scene->getName();
    yaml << YAML::Key << "Main Camera" << YAML::Value
         << toString(scene->getMainCamera().getID());
    yaml << YAML::Key << "Entities";
    yaml << YAML::BeginSeq; // Entities

    scene->eachEntity([&yaml](Entity entity) {
        if (!entity.isNull()) {
            serializeEntity(&yaml, entity);
        }
    });

    yaml << YAML::EndSeq; // Entities
    yaml << YAML::EndMap; // Scene

    if (!yaml.good()) {
        GE_CORE_ERR("YAML serialization failure: {}", yaml.GetLastError());
        return false;
    }

    fout << yaml.c_str();

    return true;
}

bool SceneSerializer::deserialize(const std::string& filepath, Scene* scene)
{
    YAML::Node yaml;

    try {
        yaml = YAML::LoadFile(filepath);
    } catch (const YAML::BadFile& e) {
        GE_CORE_ERR("Failed to load serialized Scene '{}': {}", filepath, e.what());
        return false;
    }

    auto magic_word = getYamlValue<uint32_t>(yaml, "Magic Word");

    if (magic_word != MAGIC_WORD) {
        GE_CORE_ERR("Failed to load Scene file: incompatible Scene magic word {}",
                    magic_word);
        return false;
    }

    scene->clear();

    auto scene_name = getYamlValue<std::string>(yaml, "Scene", "Untitled");
    auto main_camera_id = getYamlValue<Entity::ID>(yaml, "Main Camera", Entity::nullID());
    scene->setName(scene_name);

    if (auto entities = yaml["Entities"]; entities) {
        for (const auto& entity : entities) {
            deserializeEntity(entity, scene);
        }
    }

    scene->eachEntity([scene, main_camera_id](Entity entity) {
        if (entity.getID() == main_camera_id) {
            scene->setMainCamera(entity);
        }
    });

    return true;
}

} // namespace GE