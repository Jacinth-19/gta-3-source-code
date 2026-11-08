# External dependencies

Waypoint Engine pins third-party libraries in `dependencies.lock.json`.

Preferred release layout:

```text
external/
  entt/
  glm/
  json/
  spdlog/
  doctest/
  sdl/
  lua/
  sol2/
  jolt/
  bgfx_cmake/
  bgfx/
  bx/
  bimg/
  stb/
  tinygltf/
```

When this folder is the root of the Git repository, initialize pinned submodules with:

```bash
git submodule update --init --recursive
```

You can also create a local dependency cache from the lockfile:

```bash
python tools/wp.py deps fetch
python tools/wp.py deps status
```

`deps fetch` creates ignored local checkouts in `external/<name>`. CMake prefers those checkouts when present; otherwise it fetches the exact locked revisions with FetchContent. CI caches the FetchContent directory using the lockfile hash.

bgfx is the rendering abstraction. It can use Vulkan, Direct3D, Metal, and OpenGL backends.
