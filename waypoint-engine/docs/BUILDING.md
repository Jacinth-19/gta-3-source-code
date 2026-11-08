# Building Waypoint Engine

## Easy build

```bash
python tools/wp.py doctor
python tools/wp.py deps fetch   # optional local dependency cache
python tools/wp.py build
python tools/wp.py test
```

## Manual CMake build

```bash
cmake -S . -B build/wp -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/wp --parallel
ctest --test-dir build/wp --output-on-failure
```

## Useful options

| Option | Meaning |
|---|---|
| `WAYPOINT_BUILD_RUNTIME` | Build the game player executable. |
| `WAYPOINT_BUILD_EDITOR` | Build the Qt 6 editor. Requires Qt 6. |
| `WAYPOINT_ENABLE_RENDER` | Enable the bgfx rendering module. |
| `WAYPOINT_ENABLE_PHYSICS` | Enable Jolt Physics. |
| `WAYPOINT_ENABLE_SCRIPTING` | Enable Lua scripting. |
| `WAYPOINT_BUILD_TESTS` | Build unit tests. |
| `WAYPOINT_HEADLESS_BUILD` | Compile-check on Linux without X11/Wayland. |
| `WAYPOINT_BUILD_BGFX_TOOLS` | Build bgfx shaderc and compile runtime shaders (default ON for desktop builds). |

## Headless/container test build

On a Linux machine without X11/Wayland/OpenGL development packages, you can still build and test the non-render modules:

```bash
WAYPOINT_HEADLESS_BUILD=1 python tools/wp.py test
```

This disables the runtime and bgfx render module, but keeps core, scene, project, Lua, Jolt, and tests enabled.

## Linux desktop dependencies

For a windowed Linux build, install X11/Wayland and OpenGL development packages.
On Debian/Ubuntu-like systems:

```bash
sudo apt install build-essential ninja-build \
  libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev \
  libwayland-dev libxkbcommon-dev libegl1-mesa-dev libgl1-mesa-dev libvulkan-dev
```

For Qt 6 editor builds, install Qt 6 Widgets development packages or use the Qt online installer.

## Desktop export

After building the windowed runtime, you can make a shareable desktop folder:

```bash
python tools/wp.py export MyGame --release
```

The folder is written to `dist/<game-name>-<platform>/` and contains the runtime executable plus your project under `game/`.
Android export is still a later milestone.

## Renderer selection

bgfx chooses a backend automatically. You can request one at runtime:

```bash
WAYPOINT_RENDERER=vulkan ./build/wp/runtime/waypoint samples/hello_cube
WAYPOINT_RENDERER=opengl ./build/wp/runtime/waypoint samples/hello_cube
```

Supported values include `vulkan`, `opengl`, `opengles`, `d3d11`, `d3d12`, and `metal`, depending on platform.
