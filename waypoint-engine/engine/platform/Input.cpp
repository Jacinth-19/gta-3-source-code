#include "platform/Input.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

namespace waypoint::platform {
namespace {
std::string Normalize(std::string_view name) {
    std::string result;
    result.reserve(name.size());
    for (char ch : name) {
        if (ch == '_' || ch == '-' || ch == ' ') {
            continue;
        }
        result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return result;
}

const std::unordered_map<std::string, Key>& KeyNames() {
    static const std::unordered_map<std::string, Key> names = {
        {"a", Key::A}, {"b", Key::B}, {"c", Key::C}, {"d", Key::D}, {"e", Key::E},
        {"f", Key::F}, {"g", Key::G}, {"h", Key::H}, {"i", Key::I}, {"j", Key::J},
        {"k", Key::K}, {"l", Key::L}, {"m", Key::M}, {"n", Key::N}, {"o", Key::O},
        {"p", Key::P}, {"q", Key::Q}, {"r", Key::R}, {"s", Key::S}, {"t", Key::T},
        {"u", Key::U}, {"v", Key::V}, {"w", Key::W}, {"x", Key::X}, {"y", Key::Y},
        {"z", Key::Z},
        {"0", Key::Num0}, {"1", Key::Num1}, {"2", Key::Num2}, {"3", Key::Num3},
        {"4", Key::Num4}, {"5", Key::Num5}, {"6", Key::Num6}, {"7", Key::Num7},
        {"8", Key::Num8}, {"9", Key::Num9},
        {"space", Key::Space}, {"enter", Key::Enter}, {"return", Key::Enter},
        {"escape", Key::Escape}, {"esc", Key::Escape}, {"tab", Key::Tab},
        {"backspace", Key::Backspace},
        {"leftshift", Key::LeftShift}, {"rightshift", Key::RightShift}, {"shift", Key::LeftShift},
        {"leftcontrol", Key::LeftControl}, {"rightcontrol", Key::RightControl},
        {"leftctrl", Key::LeftControl}, {"rightctrl", Key::RightControl}, {"ctrl", Key::LeftControl},
        {"leftalt", Key::LeftAlt}, {"rightalt", Key::RightAlt}, {"alt", Key::LeftAlt},
        {"up", Key::Up}, {"down", Key::Down}, {"left", Key::Left}, {"right", Key::Right},
        {"f1", Key::F1}, {"f2", Key::F2}, {"f3", Key::F3}, {"f4", Key::F4},
        {"f5", Key::F5}, {"f6", Key::F6}, {"f7", Key::F7}, {"f8", Key::F8},
        {"f9", Key::F9}, {"f10", Key::F10}, {"f11", Key::F11}, {"f12", Key::F12},
    };
    return names;
}
} // namespace

Key KeyFromString(std::string_view name) {
    const auto normalized = Normalize(name);
    const auto it = KeyNames().find(normalized);
    return it == KeyNames().end() ? Key::Unknown : it->second;
}

const char* KeyToString(Key key) {
    switch (key) {
    case Key::A: return "A";
    case Key::B: return "B";
    case Key::C: return "C";
    case Key::D: return "D";
    case Key::E: return "E";
    case Key::F: return "F";
    case Key::G: return "G";
    case Key::H: return "H";
    case Key::I: return "I";
    case Key::J: return "J";
    case Key::K: return "K";
    case Key::L: return "L";
    case Key::M: return "M";
    case Key::N: return "N";
    case Key::O: return "O";
    case Key::P: return "P";
    case Key::Q: return "Q";
    case Key::R: return "R";
    case Key::S: return "S";
    case Key::T: return "T";
    case Key::U: return "U";
    case Key::V: return "V";
    case Key::W: return "W";
    case Key::X: return "X";
    case Key::Y: return "Y";
    case Key::Z: return "Z";
    case Key::Num0: return "0";
    case Key::Num1: return "1";
    case Key::Num2: return "2";
    case Key::Num3: return "3";
    case Key::Num4: return "4";
    case Key::Num5: return "5";
    case Key::Num6: return "6";
    case Key::Num7: return "7";
    case Key::Num8: return "8";
    case Key::Num9: return "9";
    case Key::Space: return "Space";
    case Key::Enter: return "Enter";
    case Key::Escape: return "Escape";
    case Key::Tab: return "Tab";
    case Key::Backspace: return "Backspace";
    case Key::LeftShift: return "LeftShift";
    case Key::RightShift: return "RightShift";
    case Key::LeftControl: return "LeftControl";
    case Key::RightControl: return "RightControl";
    case Key::LeftAlt: return "LeftAlt";
    case Key::RightAlt: return "RightAlt";
    case Key::Up: return "Up";
    case Key::Down: return "Down";
    case Key::Left: return "Left";
    case Key::Right: return "Right";
    case Key::F1: return "F1";
    case Key::F2: return "F2";
    case Key::F3: return "F3";
    case Key::F4: return "F4";
    case Key::F5: return "F5";
    case Key::F6: return "F6";
    case Key::F7: return "F7";
    case Key::F8: return "F8";
    case Key::F9: return "F9";
    case Key::F10: return "F10";
    case Key::F11: return "F11";
    case Key::F12: return "F12";
    default: return "Unknown";
    }
}

} // namespace waypoint::platform
