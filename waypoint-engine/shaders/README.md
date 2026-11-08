# Waypoint shaders

These are original, minimal bgfx shader sources for the first runtime milestone.

- `vs_basic.sc` / `fs_basic.sc`: position + vertex color, using bgfx's built-in `u_modelViewProj`.
- `varying.def.sc`: vertex layout shared by both stages.

They are compiled by CMake when `WAYPOINT_BUILD_BGFX_TOOLS=ON` (the default for desktop runtime builds). The compiled binaries are placed next to the runtime executable under `shaders/<profile>/`, where profile is one of `spirv`, `glsl`, `essl`, `metal`, `dxbc`, or `dxil` depending on the selected bgfx backend.

If shader binaries are missing, the runtime still opens a window and shows debug text instead of crashing.
