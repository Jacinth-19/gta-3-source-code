#pragma once

#include "ecs/World.hpp"
#include "platform/Input.hpp"

namespace waypoint::scene {

// Moves entities that have Transform + Camera + FlyCamera using WASD/Space/Ctrl
// and mouse look. Mouse look is active while relative mouse mode is on (F1) or
// while the right mouse button is held.
void UpdateFlyCameras(ecs::World& world, const platform::InputState& input, float deltaTime);

} // namespace waypoint::scene
