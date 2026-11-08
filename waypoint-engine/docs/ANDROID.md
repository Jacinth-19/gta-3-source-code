# Android export

Android export is planned through SDL3 + bgfx + the Android NDK.

Target requirements:

- Android API level 26+.
- Vulkan-capable device preferred, OpenGL ES fallback.
- CMake toolchain file from the Android NDK.
- Gradle packaging template under `templates/android`.

This is not production-ready yet. The first milestone is a native runtime library that can load a project and render a clear frame.
