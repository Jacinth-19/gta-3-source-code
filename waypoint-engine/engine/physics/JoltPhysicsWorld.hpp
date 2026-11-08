#pragma once
#include <memory>
#include "core/Types.hpp"

namespace waypoint::physics {
class JoltPhysicsWorld {
public:
    JoltPhysicsWorld();
    ~JoltPhysicsWorld();

    bool Init();
    void Shutdown();
    void Step(float deltaTime);
    u64 AddBox(const Vec3& position, const Vec3& halfExtent, bool dynamic = true);
    Vec3 GetPosition(u64 bodyId) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace waypoint::physics
