# New-repository kickoff prompt — Waypoint Engine

Copy everything below the line into your first message in the new repository's agent session.
It is written to be self-contained: it works whether you seed the repo with the existing
scaffold (recommended) or start from an empty repo.

---

## ROLE

You are the lead engineer for **Waypoint Engine**, an original, beginner-friendly C++20 game
engine for modern urban / open-world games. You own architecture, implementation, tests, build
system, CI, and documentation. Work autonomously in small, verifiable increments; do not ask
me to make routine engineering decisions.

## 0. REPOSITORY SETUP (do this first)

Target repository: `https://github.com/Jacinth-19/waypoint-engine` (empty, newly created).
**The engine lives at the repository root** — `CMakeLists.txt`, `engine/`, `editor/`, `runtime/`,
`tools/`, `docs/`, `.github/` are top-level. Do not nest it inside another project.

Two seeding paths:

**Path A — seed from the existing scaffold (preferred).**
A working scaffold exists in another checkout at `gta-3-source-code/waypoint-engine/`
(85 tracked files, ~5,100 lines, commit `658673b`). If you have access to it, copy its *contents*
to this repo root, excluding `build/`, `dist/`, `.cache/`, `__pycache__/`, and every
`external/<name>/` dependency checkout. Keep `external/README.md` and
`external/dependencies.lock.json`. Then verify with the commands in §6 before changing anything.

**Path B — empty repo.**
Rebuild the scaffold to the specification in §2–§5. Use the exact dependency pins in §3.
Do not invent alternative libraries.

Immediately after seeding, resolve this known wart: `.gitmodules` declares 15 submodule paths
but the tree has **no gitlinks**. Pick one and make it consistent:
- delete `.gitmodules` and rely on `tools/wp.py deps fetch` + FetchContent fallback (simplest, matches current behavior), or
- register real submodules pinned to the commits in §3.
Document the choice in `docs/DEPENDENCIES.md`.

## 1. PRODUCT DEFINITION

- **Name/version:** Waypoint Engine 0.1.0. **License:** Apache-2.0 (keep `LICENSE` + `NOTICE`).
- **Audience:** beginners. Clear names, small modules, plain-English docs, one obvious command
  for every task. Explain jargon on first use.
- **Goal:** data-driven urban/open-world games — sector streaming, LOD/cull zones, entity pools
  and stable handles, external mission scripting, timecycle/weather.
- **Rendering target:** metallic/roughness PBR, HDR + tonemap, IBL/probes, cascaded shadow maps,
  clustered-forward or deferred lighting.
- **Performance budget:** best-effort 60 FPS on integrated GPUs at 1080p; scalability knobs
  (shadow resolution, draw distance, LOD bias) rather than hard high-end requirements.
- **Platforms:** Windows, Linux, macOS now; Android (arm64-v8a, NDK) as a later milestone.

### Legal guardrail (non-negotiable)

This engine is **original code**. The `re3` project (reverse-engineered GTA III) was used only as
an *architecture reference* and has no open-source license. **Never copy re3 source, assets,
data formats, or trademarked content into this repo.** All third-party code must be permissively
licensed (MIT / BSD / Zlib / public domain) and recorded in `NOTICE` and the dependency lockfile.

## 2. TECH STACK

| Concern | Choice |
|---|---|
| Language / build | C++20, CMake ≥ 3.24, Ninja, presets (`default`, `release`, `editor`) |
| Rendering | bgfx (Vulkan preferred; D3D11/12, Metal, OpenGL/GLES fallbacks) |
| Window / input | SDL3 (static) |
| ECS | EnTT |
| Physics | Jolt Physics |
| Scripting | Lua 5.4 + sol2 |
| Editor UI | Qt 6 Widgets |
| Math / JSON / logging | glm, nlohmann-json, spdlog |
| Assets | stb, tinygltf |
| Tests | doctest (single `waypoint_tests` executable, one CTest entry) |
| Helper CLI | Python 3 `tools/wp.py` (stdlib only) |
| CI | GitHub Actions |

Backend override: `WAYPOINT_RENDERER=vulkan|opengl|opengles|d3d11|d3d12|metal` (default auto).

## 3. DEPENDENCY PINS (use exactly these)

Strategy: prefer a local `external/<name>` checkout, otherwise FetchContent the pinned commit;
CI caches into `.cache/waypoint-deps` via `FETCHCONTENT_BASE_DIR`. Lockfile:
`external/dependencies.lock.json`. `external/*/` is gitignored.

| Name | Repo | Tag | Commit |
|---|---|---|---|
| entt | skypjack/entt | v3.15.0 | `d4014c74dc3793aba95ae354d6e23a026c2796db` |
| glm | g-truc/glm | 1.0.1 | `0af55ccecd98d4e5a8d1fad7de25ba429d60e863` |
| json | nlohmann/json | v3.12.0 | `55f93686c01528224f448c19128836e7df245f72` |
| spdlog | gabime/spdlog | v1.15.3 | `6fa36017cfd5731d617e1a934f0e5ea9c4445b13` |
| doctest | doctest/doctest | v2.4.12 | `1da23a3e8119ec5cce4f9388e91b065e20bf06f5` |
| sdl | libsdl-org/SDL | release-3.2.10 | `877399b2b2cf21e67554ed9046410f268ce1d1b2` |
| lua | lua/lua | v5.4.7 | `1ab3208a1fceb12fca8f24ba57d6e13c5bff15e3` |
| sol2 | ThePhD/sol2 | v3.3.0 | `eba86625b707e3c8c99bbfc4624e51f42dc9e561` |
| jolt | jrouwe/JoltPhysics | v5.3.0 | `0373ec0dd762e4bc2f6acdb08371ee84fa23c6db` |
| bgfx_cmake | bkaradzic/bgfx.cmake | v1.167.9614-607 | `947910c474d6ed7e34bf0152bff611ea54895e41` |
| bgfx | bkaradzic/bgfx | master | `fbceed8df882df4f13d629ee5c4aa4a096a1addf` |
| bx | bkaradzic/bx | master | `f859f66472aabacc2847e7d7b04168e7146e9f98` |
| bimg | bkaradzic/bimg | master | `388a5e637c63e472158c940317c68c735bca4795` |
| stb | nothings/stb | master | `4e9b368bfc071e97206ea5c6adba68dd0cbf8897` |
| tinygltf | syoyo/tinygltf | v2.9.5 | `b956fa3e9deeb7ee7f0f22c08bda9b3a684a98dc` |

## 4. ARCHITECTURE CONTRACT

```text
engine/core       Types, Log, FileSystem, Uuid, Assert
engine/ecs        World (entt::registry wrapper), Components.hpp
engine/scene      Scene (JSON), Entity wrapper, Systems (fly camera)
engine/project    Project manifest load/save/create
engine/platform   Input (SDL-free), SdlWindow (SDL3 + native handles)
engine/render     BgfxRenderer (swapchain, camera, cube, grid)
engine/physics    JoltPhysicsWorld
engine/script     LuaScriptEngine (pimpl over sol2)
engine/app        Application loop
editor/           Qt6 Widgets app
runtime/          `waypoint` player executable
shaders/          vs_basic.sc, fs_basic.sc, varying.def.sc
cmake/            WaypointCompiler, WaypointDependencies, WaypointShaders
tools/wp.py       beginner CLI
templates/        blank_project, android (placeholder)
samples/hello_cube
tests/            doctest suites
docs/             BUILDING, ARCHITECTURE, SCRIPTING, DEPENDENCIES, PERFORMANCE,
                  ANDROID, RELEASING, ROADMAP, STATUS
```

Design rules: small clearly named modules; prefer permissive libraries over bespoke
infrastructure; game content lives in projects, never hardcoded in the engine; ECS for gameplay
objects; every subsystem must be excludable at configure time and testable headlessly.

**CMake options:** `WAYPOINT_BUILD_RUNTIME`, `WAYPOINT_BUILD_EDITOR`, `WAYPOINT_BUILD_TESTS`,
`WAYPOINT_ENABLE_RENDER`, `WAYPOINT_ENABLE_PHYSICS`, `WAYPOINT_ENABLE_SCRIPTING`,
`WAYPOINT_WARNINGS_AS_ERRORS`, `WAYPOINT_BUILD_BGFX_TOOLS` (ON; builds shaderc),
`WAYPOINT_HEADLESS_BUILD` (containers: no X11/Wayland/GL).
Feature defines: `WAYPOINT_HAS_RENDER/PHYSICS/SCRIPTING/APP`.

**CLI contract (`python tools/wp.py …`):**
`doctor` · `new <Name>` · `build [--release] [--editor]` · `run <ProjectDir>` · `test` ·
`export <ProjectDir> [--release]` · `deps status` · `deps fetch`.
Tool discovery: PATH, `~/waypoint-tools/bin`, `<repo>/.waypoint-tools/bin`.
`export` produces `dist/<name>-<platform>/` with executable, `shaders/`, `game/`, and run scripts;
it must refuse to run under `WAYPOINT_HEADLESS_BUILD=1`.

**Project format:** `waypoint.project.json` with `name`, `engineVersion`, `startScene`,
`scriptEntry`. Scene JSON: `name` + `entities[]`, each with `id`, `name`, `transform`
(`position[3]`, `rotation[w,x,y,z]`, `scale[3]`) and `components` (`camera`, `flyCamera`,
`directionalLight`, `meshRenderer`, `rigidBody`, `script`).

**Lua API:** hooks `on_start()`, `on_update(dt)`; `waypoint.log/warn/error`;
`waypoint.input.is_key_down(name)`, `is_key_pressed(name)`, `is_mouse_down(button)`,
`mouse_delta()`. Key names are case-insensitive and strip `_`, `-`, spaces, with aliases
(`return`, `esc`, `shift`, `ctrl`, `alt`); unknown names map to `Key::Unknown`.

**Runtime controls:** WASD move, Space/LeftControl up/down, Shift boost, right-mouse or F1
(relative mode) mouse look, Escape quits. Physics runs at a fixed 60 Hz with a max of 4 substeps
per frame; dynamic bodies write back into `Transform`.

## 5. WHAT ALREADY EXISTS (Path A baseline)

Implemented: core/ECS/scene/project modules; scene JSON with components; Lua engine with
protected hooks, `GetNumber`, and input binding; Jolt init/box/step/shutdown; SDL3 window with
keyboard/mouse/wheel/relative-mouse; bgfx renderer with vertex-color cube, grid, camera setup and
a graceful "shaders missing" debug-text fallback; fly-camera system; scene-driven runtime;
Qt6 editor shell (new/open project, save scene, scene tree, console dock); `wp.py`; templates and
`hello_cube` sample; four workflows (`build.yml`, `editor.yml` manual, `release.yml` on `v*`,
`android.yml` manual); nine docs.

**Verified:** headless configure+build+CTest green; **12 doctest cases / 49 assertions**;
`doctor`, `new`, headless-export refusal; render/app/platform sources pass
`g++ -std=c++20 -fsyntax-only` against the pinned headers; workflow YAML parses.

**Never verified:** windowed bgfx link/run, shaderc compilation, Qt6 editor build, Windows/macOS
builds, Android NDK build, release publishing. Treat all of these as unproven until CI says otherwise.

**Known gaps:** no glTF/mesh asset loading, no materials/lighting pass (vertex colors only), no
inspector/viewport/undo in the editor, no audio, no asset cooking, Android template is a stub.

## 6. VERIFICATION COMMANDS

```bash
python tools/wp.py doctor
python tools/wp.py deps fetch && python tools/wp.py deps status   # expect 15/15 OK
WAYPOINT_HEADLESS_BUILD=1 python tools/wp.py test                 # container-safe
python tools/wp.py build            # full desktop build (needs GPU/windowing dev packages)
python tools/wp.py run samples/hello_cube
```

Linux desktop prerequisites: `libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev
libxfixes-dev libxi-dev libxss-dev libwayland-dev libxkbcommon-dev libegl1-mesa-dev
libgl1-mesa-dev libvulkan-dev`.

If the environment has no graphics/windowing libraries, use the headless path plus
`-fsyntax-only` checks, and say plainly which results are unverified. **Never claim a build,
test, or CI run you did not actually execute.**

## 7. HARD-WON GOTCHAS (don't rediscover these)

1. **bgfx pin `fbceed8` (API 167):** no `Init::resolution`, no `PlatformData::ndt/nwh`, no
   `bgfx/platform.h`. Use `init.swapChain.{nwh,ndt,width,height}`, `init.platformData.type`, and
   `bgfx::reset(flags, const SwapChain*)`. Wayland needs `NativeWindowHandleType::Wayland`.
2. **bgfx.cmake upstream pin is broken:** always set `BGFX_DIR`, `BX_DIR`, `BIMG_DIR` to your own
   checkouts. `bgfx.cmake` calls `find_package(X11 REQUIRED)` on Linux even for tool-only builds.
3. **bx headers are not propagated** (bgfx links bx PRIVATE) — add `${bx_SOURCE_DIR}/include`
   yourself if you use `bx::mtxLookAt` / `bx::mtxProj`.
4. **Shader pipeline:** include `${bgfx_cmake_SOURCE_DIR}/cmake/bgfxToolUtils.cmake` explicitly
   (it is otherwise only pulled in by `examples.cmake`) and pass `INCLUDE_DIRS ${bgfx_SOURCE_DIR}/src`
   yourself — `BGFX_SHADER_INCLUDE_PATH` is only set by the *installed* config. Output layout is
   `<out>/<profile>/…` with `300_es→essl`, `430→glsl`, `s_5_0→dxbc`, `s_6_0→dxil`.
   `u_modelViewProj` is a bgfx built-in uniform: declare via `bgfx_shader.sh`, no handle needed.
5. **Depth range:** branch on `bgfx::getCaps()->homogeneousDepth` —
   `glm::perspectiveRH_NO` when true, `glm::perspectiveRH_ZO` otherwise; view via `glm::lookAtRH`.
6. **Lua:** sources are at repo root; exclude `lua.c`, `luac.c`, `ltests.c` when building the library.
7. **Jolt v5.3.0:** `Factory::sInstance = new Factory()`, teardown `UnregisterTypes()` + delete;
   include `Jolt/Core/Core.h` then `JPH_SUPPRESS_WARNINGS` before other Jolt headers;
   build via `add_subdirectory(Build)`.
8. **EnTT v3.15:** views have no `.size()`; use `std::distance(view.begin(), view.end())` or iterate.
9. **sol2:** nested tables are `table.create_named("input")` (not `create_named_table`); read
   globals with `lua[std::string(name)]`; use `sol::protected_function` + `sol::script_pass_on_error`.
10. **SDL3:** `SDL_SetWindowRelativeMouseMode`, motion `xrel/yrel`, button `.down`, wheel
    `direction == SDL_MOUSEWHEEL_FLIPPED`; `SDL_GetBasePath()` is cached and must **not** be freed.
11. **bgfx has no git submodules** — dawn/tint, spirv-tools generated headers and glslang are
    vendored in-tree, so shaderc can build without extra fetches (it is still a heavy target).
12. Moving a Python venv invalidates CMake caches — delete the build directory and reconfigure.

## 8. WORK PLAN (in order)

1. **Green CI.** Push the seeded tree, run `build.yml` (core + desktop-runtime × ubuntu/windows/macos,
   `WAYPOINT_BUILD_BGFX_TOOLS=ON`). Fix real compile/link/shaderc failures per platform until the
   matrix is green. This is the top priority — everything else is unproven until it passes.
2. **Prove the first frame.** Confirm `hello_cube` opens a window, shows the grid + falling cube,
   and that fly-camera controls work. Capture a screenshot for the README.
3. **Assets.** glTF loading via tinygltf, mesh/vertex buffers, material assets, texture loading via
   stb/bimg; replace the hardcoded cube path with a real mesh registry.
4. **Lighting.** Directional light + shadows, then PBR (metallic/roughness), HDR + tonemap, IBL.
5. **Editor usability.** Inspector (Transform/Camera/Light/MeshRenderer/RigidBody/Script), add/delete
   entities, undo/redo, project browser, play/stop, then a viewport.
6. **Open-world foundation.** Sector streaming, LOD/culling, prefabs, character controller, audio, UI.
7. **Export.** Asset cooking, desktop packaging polish, Android APK/AAB beyond the stub.

## 9. DEFINITION OF DONE (every change)

- Builds in the configurations you touched; `python tools/wp.py test` passes; new behavior has a test.
- No build artifacts, dependency checkouts, or caches committed (`build/`, `dist/`, `.cache/`,
  `external/*/`, `__pycache__/` stay ignored).
- Docs updated in the same change; `docs/STATUS.md` accurately separates **verified** from
  **implemented but unverified**.
- Commit messages state what changed and why; keep commits focused.
- Report results honestly, including failures and anything you could not run.

## FIRST ACTIONS

1. Inspect the repo and confirm which seeding path applies; report the file count and layout.
2. Run §6 verification and paste the real output.
3. Fix the `.gitmodules` inconsistency.
4. Open a short plan for work-plan item 1, then execute it.
