# Performance goal

Target: 60 FPS at 1080p and 1440p on modern integrated GPUs where possible.

This is a best-effort target, not a guarantee for every device or scene.

Planned techniques:

- bgfx backend selection: Vulkan/Direct3D/Metal/OpenGL.
- Quality profiles: low, medium, high.
- Dynamic resolution scaling.
- Frustum and distance culling.
- LOD for meshes.
- Instancing and batching.
- Baked lighting plus limited dynamic lights.
- Reflection probes instead of expensive real-time ray tracing.
- Physics sleeping and fixed timestep.
