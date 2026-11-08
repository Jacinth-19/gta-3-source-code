# Project status

Updated: 2026-11-07

## Verified locally

- `python tools/wp.py doctor` finds Git, Python, CMake, Ninja, and CTest.
- `python tools/wp.py deps fetch` / `deps status` can populate and verify the pinned `external/` cache.
- Fresh headless build: `WAYPOINT_HEADLESS_BUILD=1 python tools/wp.py test`.
- Automated tests: 12 doctest cases / 49 assertions passing.
  - UUID + filesystem helpers.
  - Project create/load.
  - Scene JSON round-trip, including camera/mesh/rigid-body components.
  - Input edge detection and beginner-friendly key names.
  - Fly-camera movement.
  - Lua start/update hooks, input queries, and syntax-error reporting.
  - Jolt gravity/falling-box smoke test.
- Syntax-checked the bgfx renderer, SDL3 window/input wrapper, runtime application, and runtime entry point against the pinned dependency headers.

## Implemented but awaiting windowed CI verification

- SDL3 keyboard/mouse input with relative mouse mode (F1) and right-mouse look.
- Fly camera: WASD, Space/Ctrl, Shift boost, pitch clamp.
- Basic bgfx draw path: vertex-color cube and editor-style grid.
- Shader sources (`shaders/*.sc`) and CMake shaderc pipeline (`WAYPOINT_BUILD_BGFX_TOOLS=ON`, default).
- Scene-driven runtime: camera, mesh renderer, and Jolt rigid bodies are loaded from scene JSON.
- Lua `waypoint.input.*` helpers.
- Export/release packaging now includes compiled shaders.

## Not yet verified locally

- Full windowed bgfx runtime link/run. This sandbox lacks X11/Wayland/OpenGL development libraries; the CI workflows install them.
- Shaderc compilation. It is wired into CMake/CI but cannot run in this sandbox.
- Qt 6 editor compile. Qt is not installed in this sandbox; the manual Editor workflow builds it.
- Windows/macOS builds and Android NDK compile. These are covered by CI matrices/manual workflows.
- Release packaging/publication. The Release workflow is ready, but it needs the standalone GitHub repository and a `v*` tag.

## Known blocker

The connected GitHub token can push the existing Arena branch, but it cannot create a new repository. `Jacinth-19/waypoint-engine` has not been created. To enable CI/releases as a clean standalone project, reconnect GitHub with repository-creation permission or manually create an empty `waypoint-engine` repository, then move/push the contents of this folder as the repository root.

## Next engineering steps

1. Run the Build workflow on the standalone repository and fix any platform-specific bgfx/SDL/Jolt/shaderc issues.
2. Add glTF mesh loading and basic material assets.
3. Expand the Qt editor from shell to hierarchy/inspector/play mode.
4. Add asset cooking and Android export beyond the placeholder template.
