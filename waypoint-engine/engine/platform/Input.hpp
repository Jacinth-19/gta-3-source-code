#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "core/Types.hpp"

namespace waypoint::platform {

// A small, engine-owned key list. SDL scancodes are translated into these values
// by SdlWindow, so gameplay code and tests do not need to include SDL headers.
enum class Key : u16 {
    Unknown = 0,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Space, Enter, Escape, Tab, Backspace,
    LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
    Up, Down, Left, Right,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    Count
};

enum class MouseButton : u8 { Left = 0, Middle = 1, Right = 2, Count = 3 };

Key KeyFromString(std::string_view name);
const char* KeyToString(Key key);

struct InputState {
    std::array<bool, static_cast<size_t>(Key::Count)> down{};
    std::array<bool, static_cast<size_t>(Key::Count)> pressed{};
    std::array<bool, static_cast<size_t>(Key::Count)> released{};
    std::array<bool, static_cast<size_t>(MouseButton::Count)> mouseDown{};
    std::array<bool, static_cast<size_t>(MouseButton::Count)> mousePressed{};
    std::array<bool, static_cast<size_t>(MouseButton::Count)> mouseReleased{};

    Vec2 mousePosition{0.0f};
    Vec2 mouseDelta{0.0f};
    Vec2 wheel{0.0f};
    bool relativeMouse = false;

    void BeginFrame() {
        pressed.fill(false);
        released.fill(false);
        mousePressed.fill(false);
        mouseReleased.fill(false);
        mouseDelta = Vec2{0.0f};
        wheel = Vec2{0.0f};
    }

    void SetKey(Key key, bool isDown) {
        const size_t index = static_cast<size_t>(key);
        if (index >= down.size() || key == Key::Unknown) {
            return;
        }
        if (isDown && !down[index]) {
            pressed[index] = true;
        }
        if (!isDown && down[index]) {
            released[index] = true;
        }
        down[index] = isDown;
    }

    void SetMouseButton(MouseButton button, bool isDown) {
        const size_t index = static_cast<size_t>(button);
        if (index >= mouseDown.size()) {
            return;
        }
        if (isDown && !mouseDown[index]) {
            mousePressed[index] = true;
        }
        if (!isDown && mouseDown[index]) {
            mouseReleased[index] = true;
        }
        mouseDown[index] = isDown;
    }

    bool IsKeyDown(Key key) const {
        const size_t index = static_cast<size_t>(key);
        return index < down.size() && down[index];
    }

    bool IsKeyPressed(Key key) const {
        const size_t index = static_cast<size_t>(key);
        return index < pressed.size() && pressed[index];
    }

    bool IsKeyReleased(Key key) const {
        const size_t index = static_cast<size_t>(key);
        return index < released.size() && released[index];
    }

    bool IsMouseDown(MouseButton button) const {
        const size_t index = static_cast<size_t>(button);
        return index < mouseDown.size() && mouseDown[index];
    }
};

} // namespace waypoint::platform
