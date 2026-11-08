#pragma once
#include <string>
#include "core/Types.hpp"

namespace waypoint::ecs {
struct Name {
    std::string value;
};

struct Transform {
    Vec3 position{0.0f};
    Quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3 scale{1.0f};

    Mat4 Matrix() const {
        const Mat4 translation = glm::translate(Mat4{1.0f}, position);
        const Mat4 rotationMatrix = glm::mat4_cast(rotation);
        const Mat4 scaleMatrix = glm::scale(Mat4{1.0f}, scale);
        return translation * rotationMatrix * scaleMatrix;
    }
};

struct Camera {
    f32 verticalFovDegrees = 60.0f;
    f32 nearPlane = 0.1f;
    f32 farPlane = 1000.0f;
    bool primary = true;
};

struct FlyCamera {
    f32 moveSpeed = 6.0f;
    f32 boostMultiplier = 3.0f;
    f32 lookSensitivity = 0.1f;
    f32 yawDegrees = 0.0f;
    f32 pitchDegrees = 0.0f;
};

struct DirectionalLight {
    Vec3 color{1.0f};
    f32 intensity = 1.0f;
};

struct MeshRenderer {
    std::string mesh;
    std::string material;
};

struct RigidBody {
    u64 bodyId = 0;
    bool dynamic = true;
    Vec3 halfExtent{0.5f};
};

struct Script {
    std::string path;
};
} // namespace waypoint::ecs
