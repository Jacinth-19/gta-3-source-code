#pragma once
#include <string>
#include "core/Types.hpp"
#include "platform/Input.hpp"

struct SDL_Window;

namespace waypoint::platform {
struct NativeWindowHandles {
    void* display = nullptr;
    void* window = nullptr;
    bool wayland = false;
};

class SdlWindow {
public:
    SdlWindow();
    ~SdlWindow();

    bool Create(const std::string& title, int width, int height);
    void Destroy();
    bool ShouldClose() const;
    void PollEvents();
    NativeWindowHandles GetNativeHandles() const;
    void GetDrawableSize(int& width, int& height) const;

    const InputState& Input() const { return input_; }
    InputState& Input() { return input_; }
    void SetRelativeMouseMode(bool enabled);
    bool RelativeMouseMode() const { return input_.relativeMouse; }

private:
    SDL_Window* window_ = nullptr;
    bool shouldClose_ = false;
    InputState input_;
};
} // namespace waypoint::platform
