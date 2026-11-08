#include <doctest/doctest.h>

#include "script/LuaScriptEngine.hpp"

TEST_CASE("lua runs start/update hooks and exposes waypoint table") {
    waypoint::script::LuaScriptEngine script;
    REQUIRE(script.LoadString(R"(
        counter = 0
        function on_start()
            waypoint.log("hello from test")
            counter = 10
        end
        function on_update(dt)
            counter = counter + dt
        end
    )"));
    script.CallStart();
    CHECK(script.GetNumber("counter").value_or(-1.0) == doctest::Approx(10.0));
    script.CallUpdate(0.5f);
    CHECK(script.GetNumber("counter").value_or(-1.0) == doctest::Approx(10.5));
    CHECK(script.LastError().empty());
}

TEST_CASE("lua can query bound input state") {
    waypoint::platform::InputState input;
    input.SetKey(waypoint::platform::Key::W, true);
    waypoint::script::LuaScriptEngine script;
    script.BindInput(&input);
    REQUIRE(script.LoadString(R"(
        function on_start()
            moving = waypoint.input.is_key_down("w") and 1 or 0
        end
        function on_update(dt)
            value = moving + dt
        end
    )"));
    script.CallStart();
    script.CallUpdate(0.25f);
    CHECK(script.GetNumber("value").value_or(-1.0) == doctest::Approx(1.25f));
    CHECK(script.LastError().empty());
}

TEST_CASE("lua reports syntax errors") {
    waypoint::script::LuaScriptEngine script;
    CHECK_FALSE(script.LoadString("this is not lua"));
    CHECK_FALSE(script.LastError().empty());
}
