#pragma once
#include <entt/entt.hpp>
#include "ecs/Components.hpp"
#include "ecs/World.hpp"

namespace waypoint::scene {
class Entity {
public:
    Entity() = default;
    Entity(ecs::World& world, entt::entity handle) : world_(&world), handle_(handle) {}

    bool Valid() const { return world_ && world_->Valid(handle_); }
    entt::entity Handle() const { return handle_; }

    template <typename T, typename... Args>
    T& Add(Args&&... args) {
        return world_->Registry().emplace_or_replace<T>(handle_, std::forward<Args>(args)...);
    }

    template <typename T>
    bool Has() const {
        return world_->Registry().all_of<T>(handle_);
    }

    template <typename T>
    T& Get() {
        return world_->Registry().get<T>(handle_);
    }

    template <typename T>
    const T& Get() const {
        return world_->Registry().get<T>(handle_);
    }

private:
    ecs::World* world_ = nullptr;
    entt::entity handle_ = entt::null;
};
} // namespace waypoint::scene
