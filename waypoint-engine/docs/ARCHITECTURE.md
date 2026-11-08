# Architecture

Waypoint Engine is split into small modules so beginners can find things quickly.

```text
engine/core       Basic types, logging, files, IDs
engine/ecs        EnTT world and gameplay components
engine/scene      Scene loading/saving and entities
engine/project    Project manifest and templates
engine/platform   SDL3 window and native handles
engine/render     bgfx renderer
engine/physics    Jolt Physics wrapper
engine/script     Lua scripting wrapper
engine/app        Runtime application loop
editor            Qt 6 editor
runtime           Game player executable
```

## Design rules

1. Keep modules small and named clearly.
2. Prefer existing permissive libraries over custom infrastructure.
3. Keep game content in projects, not hardcoded in the engine.
4. Use ECS for gameplay objects.
5. Use bgfx so the engine can target Vulkan, Direct3D, Metal, and OpenGL.
6. Keep performance scalable for integrated GPUs.
