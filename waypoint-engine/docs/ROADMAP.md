# Roadmap

## Milestone 0: scaffold (done)

- C++20 engine library.
- Core, ECS, scene, project, Lua, and Jolt modules.
- bgfx renderer wrapper and SDL3 window wrapper.
- Qt 6 editor shell.
- Beginner helper CLI (`doctor`, `new`, `build`, `run`, `test`, `export`, `deps`).
- Dependency lockfile, local dependency cache, and CI workflows with cache/release publishing.
- Unit tests for core/project/scene/Lua/Jolt.

## Milestone 1: first playable runtime (in progress)

- Validate bgfx runtime on Windows, Linux, and macOS.
- Shader compilation pipeline with bgfx shaderc (implemented; awaiting CI).
- Render a simple vertex-color cube and grid (implemented; awaiting CI).
- Camera and input mapping (implemented: fly camera, WASD/mouse, Lua input queries).
- Scene-driven rigid bodies and mesh renderers (implemented).
- Load glTF meshes with tinygltf.
- Add basic material assets and lighting/PBR pass.

## Milestone 2: editor usability

- Scene hierarchy editing.
- Inspector for Transform, Camera, Light, MeshRenderer, Script.
- Undo/redo.
- Project browser.
- Play/stop from editor.
- Viewport rendering through bgfx or Qt RHI bridge.

## Milestone 3: open-world foundation

- Sector streaming.
- LOD and culling.
- Prefabs.
- Physics bodies and character controller.
- Audio backend.
- UI system.

## Milestone 4: export

- Windows/Linux/macOS game packaging.
- Android APK/AAB export.
- Asset cooking.
- Release installer/zip templates.
