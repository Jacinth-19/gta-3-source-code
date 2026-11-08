# Waypoint Engine

Waypoint Engine is a beginner-friendly C++20 game engine for modern urban/open-world games.

It uses **bgfx** for rendering. bgfx is a rendering abstraction that can use **Vulkan**, Direct3D, Metal, and OpenGL. That means the engine can prefer Vulkan where it works well, while still falling back to other backends when needed.

## What is inside?

- **C++20** engine code.
- **bgfx** rendering with Vulkan/Direct3D/Metal/OpenGL backends.
- **Qt 6** editor UI.
- **EnTT** ECS.
- **Jolt Physics** physics.
- **Lua** scripting.
- **SDL3** window/input/platform layer.
- **CMake + Ninja** build system.
- **GitHub Actions** for Windows, Linux, and macOS builds.
- Android export template placeholder for a later milestone.

> Status: early scaffold. Core, project, scene, Lua, and Jolt modules build and pass automated tests locally. The bgfx runtime is wired for the pinned bgfx API, but full windowed desktop validation depends on platform graphics/windowing dependencies and CI.

## Quick start

```bash
python tools/wp.py doctor
python tools/wp.py deps fetch   # optional: cache pinned dependencies locally
python tools/wp.py new MyGame
python tools/wp.py build
python tools/wp.py run MyGame
```

## Folder map

```text
engine/      Engine modules: core, ECS, scene, project, render, physics, script, platform, app
editor/      Qt 6 editor
runtime/     Game player executable
external/    Pinned third-party dependencies
templates/   New project templates
samples/     Example projects
tests/       Automated tests
tools/       Beginner-friendly helper commands
docs/        Guides
```

## Easy commands

| Command | What it does |
|---|---|
| `python tools/wp.py doctor` | Checks your build tools. |
| `python tools/wp.py deps fetch` | Downloads the pinned dependency cache. |
| `python tools/wp.py new MyGame` | Creates a new game project. |
| `python tools/wp.py build` | Builds the engine and runtime. |
| `python tools/wp.py build --editor` | Builds the Qt 6 editor too. |
| `python tools/wp.py run MyGame` | Runs a game project. |
| `python tools/wp.py test` | Runs automated tests. |
| `python tools/wp.py export MyGame --release` | Makes a desktop folder you can share. |

## Performance goal

Target: **60 FPS at 1080p and 1440p on modern integrated GPUs where possible**.
This is a best-effort target, not a universal guarantee. The engine will use quality profiles, dynamic resolution, LOD, culling, and scalable rendering settings.

## Documentation

- [Building](docs/BUILDING.md)
- [Architecture](docs/ARCHITECTURE.md)
- [Scripting](docs/SCRIPTING.md)
- [Dependencies](docs/DEPENDENCIES.md)
- [Performance](docs/PERFORMANCE.md)
- [Android](docs/ANDROID.md)
- [Releasing](docs/RELEASING.md)
- [Roadmap](docs/ROADMAP.md)

## License

Engine code: **Apache-2.0**.
Third-party libraries keep their own licenses. Qt 6 has separate LGPL/GPL/commercial obligations.
