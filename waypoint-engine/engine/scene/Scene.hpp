#pragma once
#include <string>
#include "ecs/World.hpp"
#include "scene/Entity.hpp"

namespace waypoint::scene {
class Scene {
public:
    explicit Scene(std::string name = "Scene");

    const std::string& Name() const;
    void SetName(std::string name);

    Entity CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(Entity entity);
    ecs::World& World();
    const ecs::World& World() const;

    std::string ToJson() const;
    bool FromJson(std::string_view json);

private:
    std::string name_;
    ecs::World world_;
};
} // namespace waypoint::scene
