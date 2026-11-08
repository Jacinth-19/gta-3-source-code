#include "scene/Scene.hpp"

#include <nlohmann/json.hpp>

#include "core/Uuid.hpp"

namespace waypoint::scene {
using nlohmann::json;

namespace {
json Vec3ToJson(const Vec3& value) {
    return json{value.x, value.y, value.z};
}

Vec3 Vec3FromJson(const json& value, const Vec3& fallback = Vec3{0.0f}) {
    if (value.is_array() && value.size() == 3) {
        return Vec3{value[0].get<f32>(), value[1].get<f32>(), value[2].get<f32>()};
    }
    return fallback;
}

json ToJson(const ecs::Transform& transform) {
    return json{
        {"position", Vec3ToJson(transform.position)},
        {"rotation", {transform.rotation.w, transform.rotation.x, transform.rotation.y, transform.rotation.z}},
        {"scale", Vec3ToJson(transform.scale)}
    };
}

ecs::Transform TransformFromJson(const json& value) {
    ecs::Transform transform;
    if (value.contains("position")) {
        transform.position = Vec3FromJson(value["position"]);
    }
    if (value.contains("rotation") && value["rotation"].is_array() && value["rotation"].size() == 4) {
        transform.rotation = Quat{value["rotation"][0].get<f32>(), value["rotation"][1].get<f32>(),
                                  value["rotation"][2].get<f32>(), value["rotation"][3].get<f32>()};
    }
    if (value.contains("scale")) {
        transform.scale = Vec3FromJson(value["scale"], Vec3{1.0f});
    }
    return transform;
}

json ComponentsToJson(const ecs::World& world, entt::entity entity) {
    json components = json::object();
    auto& registry = world.Registry();
    if (registry.all_of<ecs::Camera>(entity)) {
        const auto& camera = registry.get<ecs::Camera>(entity);
        components["camera"] = json{
            {"verticalFovDegrees", camera.verticalFovDegrees},
            {"nearPlane", camera.nearPlane},
            {"farPlane", camera.farPlane},
            {"primary", camera.primary}
        };
    }
    if (registry.all_of<ecs::FlyCamera>(entity)) {
        const auto& fly = registry.get<ecs::FlyCamera>(entity);
        components["flyCamera"] = json{
            {"moveSpeed", fly.moveSpeed},
            {"boostMultiplier", fly.boostMultiplier},
            {"lookSensitivity", fly.lookSensitivity},
            {"yawDegrees", fly.yawDegrees},
            {"pitchDegrees", fly.pitchDegrees}
        };
    }
    if (registry.all_of<ecs::DirectionalLight>(entity)) {
        const auto& light = registry.get<ecs::DirectionalLight>(entity);
        components["directionalLight"] = json{
            {"color", Vec3ToJson(light.color)},
            {"intensity", light.intensity}
        };
    }
    if (registry.all_of<ecs::MeshRenderer>(entity)) {
        const auto& mesh = registry.get<ecs::MeshRenderer>(entity);
        components["meshRenderer"] = json{{"mesh", mesh.mesh}, {"material", mesh.material}};
    }
    if (registry.all_of<ecs::RigidBody>(entity)) {
        const auto& body = registry.get<ecs::RigidBody>(entity);
        components["rigidBody"] = json{
            {"dynamic", body.dynamic},
            {"halfExtent", Vec3ToJson(body.halfExtent)}
        };
    }
    if (registry.all_of<ecs::Script>(entity)) {
        components["script"] = json{{"path", registry.get<ecs::Script>(entity).path}};
    }
    return components;
}

void ComponentsFromJson(Entity entity, const json& components) {
    if (!components.is_object()) {
        return;
    }
    if (components.contains("camera")) {
        const auto& value = components["camera"];
        auto& camera = entity.Add<ecs::Camera>();
        camera.verticalFovDegrees = value.value("verticalFovDegrees", camera.verticalFovDegrees);
        camera.nearPlane = value.value("nearPlane", camera.nearPlane);
        camera.farPlane = value.value("farPlane", camera.farPlane);
        camera.primary = value.value("primary", camera.primary);
    }
    if (components.contains("flyCamera")) {
        const auto& value = components["flyCamera"];
        auto& fly = entity.Add<ecs::FlyCamera>();
        fly.moveSpeed = value.value("moveSpeed", fly.moveSpeed);
        fly.boostMultiplier = value.value("boostMultiplier", fly.boostMultiplier);
        fly.lookSensitivity = value.value("lookSensitivity", fly.lookSensitivity);
        fly.yawDegrees = value.value("yawDegrees", fly.yawDegrees);
        fly.pitchDegrees = value.value("pitchDegrees", fly.pitchDegrees);
    }
    if (components.contains("directionalLight")) {
        const auto& value = components["directionalLight"];
        auto& light = entity.Add<ecs::DirectionalLight>();
        if (value.contains("color")) {
            light.color = Vec3FromJson(value["color"], light.color);
        }
        light.intensity = value.value("intensity", light.intensity);
    }
    if (components.contains("meshRenderer")) {
        const auto& value = components["meshRenderer"];
        auto& mesh = entity.Add<ecs::MeshRenderer>();
        mesh.mesh = value.value("mesh", std::string{});
        mesh.material = value.value("material", std::string{});
    }
    if (components.contains("rigidBody")) {
        const auto& value = components["rigidBody"];
        auto& body = entity.Add<ecs::RigidBody>();
        body.dynamic = value.value("dynamic", body.dynamic);
        if (value.contains("halfExtent")) {
            body.halfExtent = Vec3FromJson(value["halfExtent"], body.halfExtent);
        }
    }
    if (components.contains("script")) {
        auto& script = entity.Add<ecs::Script>();
        script.path = components["script"].value("path", std::string{});
    }
}
} // namespace

Scene::Scene(std::string name) : name_(std::move(name)) {}

const std::string& Scene::Name() const {
    return name_;
}

void Scene::SetName(std::string name) {
    name_ = std::move(name);
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity(world_, world_.Create());
    entity.Add<ecs::Name>(name);
    entity.Add<ecs::Transform>();
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    if (entity.Valid()) {
        world_.Destroy(entity.Handle());
    }
}

ecs::World& Scene::World() {
    return world_;
}

const ecs::World& Scene::World() const {
    return world_;
}

std::string Scene::ToJson() const {
    json root;
    root["name"] = name_;
    root["entities"] = json::array();

    auto& registry = world_.Registry();
    const auto view = registry.view<const ecs::Name, const ecs::Transform>();
    for (auto entity : view) {
        const auto& name = view.get<const ecs::Name>(entity);
        const auto& transform = view.get<const ecs::Transform>(entity);
        root["entities"].push_back(json{
            {"id", static_cast<u32>(entt::to_integral(entity))},
            {"name", name.value},
            {"transform", ::waypoint::scene::ToJson(transform)},
            {"components", ComponentsToJson(world_, entity)}
        });
    }
    return root.dump(2);
}

bool Scene::FromJson(std::string_view text) {
    json root;
    try {
        root = json::parse(text);
    } catch (...) {
        return false;
    }

    world_ = ecs::World{};
    name_ = root.value("name", "Scene");
    if (!root.contains("entities") || !root["entities"].is_array()) {
        return true;
    }

    for (const auto& item : root["entities"]) {
        Entity entity = CreateEntity(item.value("name", "Entity"));
        if (item.contains("transform")) {
            entity.Get<ecs::Transform>() = TransformFromJson(item["transform"]);
        }
        if (item.contains("components")) {
            ComponentsFromJson(entity, item["components"]);
        }
    }
    return true;
}
} // namespace waypoint::scene
