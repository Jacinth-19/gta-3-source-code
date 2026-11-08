# Dependencies

Waypoint Engine pins dependencies in `external/dependencies.lock.json`.

| Library | Purpose | License |
|---|---|---|
| EnTT | ECS | MIT |
| glm | Math | MIT |
| nlohmann/json | JSON | MIT |
| spdlog | Logging | MIT |
| doctest | Tests | MIT |
| SDL3 | Window/input/platform | Zlib |
| Lua | Scripting | MIT |
| sol2 | C++/Lua binding | MIT |
| Jolt Physics | Physics | MIT |
| bgfx/bx/bimg | Rendering abstraction | BSD-2-Clause |
| bgfx.cmake | CMake build for bgfx | BSD-2-Clause |
| stb | Image helpers | MIT/Public Domain |
| tinygltf | glTF loading | MIT |
| Qt 6 | Editor UI | LGPL/GPL/commercial |

The build prefers `external/<name>` submodules/local checkouts when present. Otherwise it fetches the locked revision with CMake FetchContent.

## Easy dependency commands

```bash
python tools/wp.py deps status  # check local external/ cache
python tools/wp.py deps fetch   # download locked revisions into external/
```

The local `external/<name>` checkouts created by `deps fetch` are ignored by Git. In the standalone engine repository, the intended release layout is pinned Git submodules plus the same lockfile.

## CI cache

GitHub Actions caches `.cache/waypoint-deps` using a key derived from `external/dependencies.lock.json`. Changing a pinned revision invalidates the cache.
