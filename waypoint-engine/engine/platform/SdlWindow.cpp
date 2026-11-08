#include "platform/SdlWindow.hpp"

#include <SDL3/SDL.h>

#include "core/Log.hpp"

namespace waypoint::platform {
namespace {
Key TranslateScancode(SDL_Scancode scancode) {
    switch (scancode) {
    case SDL_SCANCODE_A: return Key::A;
    case SDL_SCANCODE_B: return Key::B;
    case SDL_SCANCODE_C: return Key::C;
    case SDL_SCANCODE_D: return Key::D;
    case SDL_SCANCODE_E: return Key::E;
    case SDL_SCANCODE_F: return Key::F;
    case SDL_SCANCODE_G: return Key::G;
    case SDL_SCANCODE_H: return Key::H;
    case SDL_SCANCODE_I: return Key::I;
    case SDL_SCANCODE_J: return Key::J;
    case SDL_SCANCODE_K: return Key::K;
    case SDL_SCANCODE_L: return Key::L;
    case SDL_SCANCODE_M: return Key::M;
    case SDL_SCANCODE_N: return Key::N;
    case SDL_SCANCODE_O: return Key::O;
    case SDL_SCANCODE_P: return Key::P;
    case SDL_SCANCODE_Q: return Key::Q;
    case SDL_SCANCODE_R: return Key::R;
    case SDL_SCANCODE_S: return Key::S;
    case SDL_SCANCODE_T: return Key::T;
    case SDL_SCANCODE_U: return Key::U;
    case SDL_SCANCODE_V: return Key::V;
    case SDL_SCANCODE_W: return Key::W;
    case SDL_SCANCODE_X: return Key::X;
    case SDL_SCANCODE_Y: return Key::Y;
    case SDL_SCANCODE_Z: return Key::Z;
    case SDL_SCANCODE_0: return Key::Num0;
    case SDL_SCANCODE_1: return Key::Num1;
    case SDL_SCANCODE_2: return Key::Num2;
    case SDL_SCANCODE_3: return Key::Num3;
    case SDL_SCANCODE_4: return Key::Num4;
    case SDL_SCANCODE_5: return Key::Num5;
    case SDL_SCANCODE_6: return Key::Num6;
    case SDL_SCANCODE_7: return Key::Num7;
    case SDL_SCANCODE_8: return Key::Num8;
    case SDL_SCANCODE_9: return Key::Num9;
    case SDL_SCANCODE_SPACE: return Key::Space;
    case SDL_SCANCODE_RETURN: return Key::Enter;
    case SDL_SCANCODE_ESCAPE: return Key::Escape;
    case SDL_SCANCODE_TAB: return Key::Tab;
    case SDL_SCANCODE_BACKSPACE: return Key::Backspace;
    case SDL_SCANCODE_LSHIFT: return Key::LeftShift;
    case SDL_SCANCODE_RSHIFT: return Key::RightShift;
    case SDL_SCANCODE_LCTRL: return Key::LeftControl;
    case SDL_SCANCODE_RCTRL: return Key::RightControl;
    case SDL_SCANCODE_LALT: return Key::LeftAlt;
    case SDL_SCANCODE_RALT: return Key::RightAlt;
    case SDL_SCANCODE_UP: return Key::Up;
    case SDL_SCANCODE_DOWN: return Key::Down;
    case SDL_SCANCODE_LEFT: return Key::Left;
    case SDL_SCANCODE_RIGHT: return Key::Right;
    case SDL_SCANCODE_F1: return Key::F1;
    case SDL_SCANCODE_F2: return Key::F2;
    case SDL_SCANCODE_F3: return Key::F3;
    case SDL_SCANCODE_F4: return Key::F4;
    case SDL_SCANCODE_F5: return Key::F5;
    case SDL_SCANCODE_F6: return Key::F6;
    case SDL_SCANCODE_F7: return Key::F7;
    case SDL_SCANCODE_F8: return Key::F8;
    case SDL_SCANCODE_F9: return Key::F9;
    case SDL_SCANCODE_F10: return Key::F10;
    case SDL_SCANCODE_F11: return Key::F11;
    case SDL_SCANCODE_F12: return Key::F12;
    default: return Key::Unknown;
    }
}

MouseButton TranslateMouseButton(Uint8 button) {
    switch (button) {
    case SDL_BUTTON_LEFT: return MouseButton::Left;
    case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
    case SDL_BUTTON_RIGHT: return MouseButton::Right;
    default: return MouseButton::Count;
    }
}
} // namespace

SdlWindow::SdlWindow() = default;

SdlWindow::~SdlWindow() {
    Destroy();
}

bool SdlWindow::Create(const std::string& title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        log::Error(std::string("SDL_Init failed: ") + SDL_GetError());
        return false;
    }
    window_ = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
    if (!window_) {
        log::Error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
        return false;
    }
    return true;
}

void SdlWindow::Destroy() {
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();
    }
}

bool SdlWindow::ShouldClose() const {
    return shouldClose_;
}

void SdlWindow::PollEvents() {
    input_.BeginFrame();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            shouldClose_ = true;
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            const Key key = TranslateScancode(event.key.scancode);
            input_.SetKey(key, event.key.down);
            if (event.key.down && key == Key::Escape) {
                shouldClose_ = true;
            }
            if (event.key.down && key == Key::F1) {
                SetRelativeMouseMode(!input_.relativeMouse);
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION:
            input_.mousePosition = Vec2{event.motion.x, event.motion.y};
            input_.mouseDelta += Vec2{event.motion.xrel, event.motion.yrel};
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            const MouseButton button = TranslateMouseButton(event.button.button);
            if (button != MouseButton::Count) {
                input_.SetMouseButton(button, event.button.down);
            }
            input_.mousePosition = Vec2{event.button.x, event.button.y};
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            const float flip = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1.0f : 1.0f;
            input_.wheel += Vec2{event.wheel.x * flip, event.wheel.y * flip};
            break;
        }
        default:
            break;
        }
    }
}

void SdlWindow::SetRelativeMouseMode(bool enabled) {
    if (window_) {
        SDL_SetWindowRelativeMouseMode(window_, enabled);
    }
    input_.relativeMouse = enabled;
}

NativeWindowHandles SdlWindow::GetNativeHandles() const {
    NativeWindowHandles handles;
    if (!window_) {
        return handles;
    }
    SDL_PropertiesID props = SDL_GetWindowProperties(window_);
#if defined(SDL_PLATFORM_WIN32)
    handles.window = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(SDL_PLATFORM_MACOS)
    handles.window = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif defined(SDL_PLATFORM_LINUX)
    handles.display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
    handles.window = reinterpret_cast<void*>(
        static_cast<uintptr_t>(SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0)));
    if (!handles.window) {
        handles.display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        handles.window = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
        handles.wayland = handles.window != nullptr;
    }
#elif defined(SDL_PLATFORM_ANDROID)
    handles.window = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, nullptr);
#endif
    return handles;
}

void SdlWindow::GetDrawableSize(int& width, int& height) const {
    if (window_) {
        SDL_GetWindowSizeInPixels(window_, &width, &height);
    }
}
} // namespace waypoint::platform
