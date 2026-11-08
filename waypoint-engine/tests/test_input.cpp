#include <doctest/doctest.h>

#include "platform/Input.hpp"

TEST_CASE("input tracks key presses and releases") {
    waypoint::platform::InputState input;
    input.BeginFrame();
    input.SetKey(waypoint::platform::Key::W, true);
    CHECK(input.IsKeyDown(waypoint::platform::Key::W));
    CHECK(input.IsKeyPressed(waypoint::platform::Key::W));
    CHECK_FALSE(input.IsKeyReleased(waypoint::platform::Key::W));

    input.BeginFrame();
    CHECK(input.IsKeyDown(waypoint::platform::Key::W));
    CHECK_FALSE(input.IsKeyPressed(waypoint::platform::Key::W));

    input.SetKey(waypoint::platform::Key::W, false);
    CHECK_FALSE(input.IsKeyDown(waypoint::platform::Key::W));
    CHECK(input.IsKeyReleased(waypoint::platform::Key::W));
}

TEST_CASE("key names are beginner friendly") {
    CHECK(waypoint::platform::KeyFromString("w") == waypoint::platform::Key::W);
    CHECK(waypoint::platform::KeyFromString("Left_Shift") == waypoint::platform::Key::LeftShift);
    CHECK(waypoint::platform::KeyFromString("space") == waypoint::platform::Key::Space);
    CHECK(waypoint::platform::KeyFromString("not-a-key") == waypoint::platform::Key::Unknown);
}
