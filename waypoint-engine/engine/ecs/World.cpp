#include "ecs/World.hpp"

namespace waypoint::ecs {
entt::entity World::Create() {
    return registry_.create();
}

void World::Destroy(entt::entity entity) {
    if (Valid(entity)) {
        registry_.destroy(entity);
    }
}

bool World::Valid(entt::entity entity) const {
    return registry_.valid(entity);
}

entt::registry& World::Registry() {
    return registry_;
}

const entt::registry& World::Registry() const {
    return registry_;
}
} // namespace waypoint::ecs
