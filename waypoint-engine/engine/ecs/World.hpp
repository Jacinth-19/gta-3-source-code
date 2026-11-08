#pragma once
#include <entt/entt.hpp>

namespace waypoint::ecs {
class World {
public:
    entt::entity Create();
    void Destroy(entt::entity entity);
    bool Valid(entt::entity entity) const;
    entt::registry& Registry();
    const entt::registry& Registry() const;

private:
    entt::registry registry_;
};
} // namespace waypoint::ecs
