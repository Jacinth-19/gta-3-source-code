# Repository feature inventory

This document consolidates the feature list for this checkout of `Jacinth-19/gta-3-source-code`.

- Project: **re3**, a reverse-engineered Grand Theft Auto III engine.
- Branch inspected: `arena/677b6050-gta-3-source-code`.
- Base commit inspected: `310dd8637147c4db643107b69d603902abc78141`.
- Method: static inspection of `README.md`, `src/core/config.h`, build files, CI workflows, and subsystem sources.
- Important: this is **not** a build or runtime verification report. Many features are compile-time, platform-dependent, asset-dependent, or explicitly experimental.

The current checkout contains the **GTA III** code. The README also mentions GTA Vice City, but that lives on a separate `miami` branch and is not part of this checkout.

---

## 1. Core gameplay and mission systems

- Single-player open-world GTA III runtime.
- Liberty City island/level handling and transitions.
- Original SCM mission-script loading and execution.
- Mission launching, mission cleanup, objectives, timers, conditions, and scripted events.
- Story missions, cutscenes, mission briefs, statistics, and completion tracking.
- Side activities:
  - Taxi Driver
  - Paramedic
  - Firefighter
  - Vigilante
  - RC Toyz missions:
    - RC Diablo Destruction
    - RC Mafia Massacre
    - RC Rumpo Rampage
    - RC Casino Calamity
  - Patriot Playground
  - A Ride in the Park
  - Gripped!
  - Multistorey Mayhem
  - Rampages / kill frenzies
  - Marty Chonks payphone-style missions listed in the debug mission switcher:
    - The Crook
    - The Thieves
    - The Wife
    - Her Lover
- Hidden packages, unique stunt jumps, and vehicle import/export collections.
- Money, health, armour, weapon, adrenaline, and police-bribe pickups.
- Payphones, moving bridges, roadblocks, and scripted pickup cameras.
- Hospital and police-station restart points after death or arrest.
- Vehicle garages, hideout storage, respray shops, bomb shops, collection garages, cranes, and crushers.
- Alternative free-roam and debug mission scripts included under `gamefiles/data/`.
- Mission retry / quicksave support when `MISSION_REPLAY` is enabled.
- Debug mission switcher when `DEBUGMENU` and `MISSION_SWITCHER` are enabled.

Main sources: `src/control/`, `src/core/`, `gamefiles/data/`

## 2. Player, pedestrian, gang, police, and traffic behaviour

- On-foot movement, sprinting, jumping, fighting, aiming, and vehicle entry/exit.
- Melee and firearm combat, target locking, target switching, and drive-by shooting.
- Civilian, gang, police, medic, and fireman AI.
- Pedestrian pathfinding, obstacle avoidance, reactions, conversations, and event handling.
- Gang-specific pedestrian models, weapons, vehicles, and hostility data.
- Police crime detection, pursuit, arrest behaviour, and wanted-level escalation.
- Police, SWAT, FBI, army, and helicopter response logic.
- Traffic generation, lane following, traffic lights, autopilot, and parked-vehicle generation.
- Accidents, fires, and emergency-service responses.
- Skeletal animation blending and pedestrian inverse kinematics.
- Optional skinned pedestrian geometry when `PED_SKIN` is enabled.
- Cancellable vehicle-entry animations when `CANCELLABLE_CAR_ENTER` is enabled.
- Animated pedestrian collision models when `ANIMATE_PED_COL_MODEL` is enabled.
- Extended off-screen despawn ranges when `EXTENDED_OFFSCREEN_DESPAWN_RANGE` is enabled.
- Pedestrian and traffic density multipliers, exposed through frontend sliders in custom builds.

Main sources: `src/peds/`, `src/control/CarAI.cpp`, `src/control/CarCtrl.cpp`, `src/control/PathFind.cpp`, `src/core/Wanted.cpp`, `src/core/EventList.cpp`

## 3. Vehicles and physics

- Car acceleration, gearing, steering, braking, handbrake, traction, suspension, and buoyancy.
- Vehicle damage covering doors, panels, bumpers, lights, wheels, engine health, fires, and explosions.
- Detachable vehicle components and burst tyres.
- Boat handling, water resistance, buoyancy, and wakes.
- Trains with tracks, stations, doors, passengers, and collision handling.
- Dodo aircraft behaviour and background aircraft flight paths.
- Helicopter AI and scripted helicopter behaviour.
- RC Bandit remote-control vehicles.
- Tank controls, fire-truck water cannons, bus doors, taxi lights, horns, and sirens.
- Timed, ignition-triggered, and remote car bombs.
- Collision detection using spheres, boxes, lines, and triangles.
- Physical objects, projectiles, gravity, friction, and surface-dependent behaviour.
- Optional ability to destroy the Airtrain when `EXPLODING_AIRTRAIN` is enabled.
- Vehicle spawning, repair, road placement, collision toggling, and colour controls through the debug menu.

Main sources: `src/vehicles/`, `src/entities/`, `src/collision/`, `src/objects/`, `src/core/re3.cpp`

## 4. Weapons and combat effects

- Unarmed and baseball-bat melee combat.
- Pistol, Uzi, shotgun, AK-47, M16, sniper rifle, rocket launcher, flamethrower, Molotovs, grenades, and detonator.
- Melee, instant-hit, projectile, and area-effect weapon handling.
- Ammunition, reloading, weapon switching, and weapon pickups.
- Sniper zoom and scoped aiming behaviour.
- Uzi drive-by shooting.
- Explosions, fires, bullet traces, muzzle effects, and weapon particle effects.
- Vehicle-mounted and helicopter weapons.
- Weapon cheats and debug weapon grants.

Main sources: `src/weapons/`, `src/renderer/WeaponEffects.cpp`, `src/core/Pad.cpp`

## 5. World management and rendering

- Original RenderWare / Direct3D 8 rendering path.
- librw rendering path supporting Direct3D 9 and OpenGL.
- OpenGL ES support through librw, as documented in the README.
- Model, texture, collision, and animation streaming.
- IMG archive loading and TXD texture-cache generation.
- LOD handling, visibility lists, sector scanning, and cull zones.
- Day/night lighting and time-cycle colouring.
- Sunny, cloudy, rainy, and foggy weather.
- Clouds, lightning, rain, wind, wet-road effects, and water rendering.
- Water waves and boat wakes.
- Shadows, coronas, point lights, and vehicle lighting.
- Breakable glass and glass reflections.
- Smoke, sparks, fire, splashes, debris, litter, skid marks, and motion-blur streaks.
- Widescreen and arbitrary-aspect-ratio scaling for HUD, menus, and field of view.
- Native-resolution detection, resolution selection, and windowed-mode support.
- Brightness, draw-distance, frame-limiter, and VSync controls.
- MSAA option where supported by the rendering backend.
- Anisotropic filtering when `ANISOTROPIC_FILTERING` is enabled.
- PS2-style vehicle reflections, alpha testing, and particles.
- Console-inspired vehicle rendering, pedestrian rim lighting, and screen rain droplets.
- World lightmap support with compatible Xbox map assets.
- Optional Neo rendering pipeline with configurable vehicle shininess, specularity, rim lighting, lightmaps, and road gloss.
- Selectable colour filters and adjustable post-processing intensity.
- Optional loading-screen removal.
- Configurable island loading to reduce or avoid loading screens between islands.
- Loading of compatible DFF and TXD assets from other platforms.
- Optional new librw-based world renderer when `NEW_RENDERER` is enabled.

Main sources: `src/renderer/`, `src/rw/`, `src/extras/`, `src/core/Streaming.cpp`, `src/core/FileLoader.cpp`

## 6. Audio

- Miles Sound System backend.
- OpenAL backend.
- Silent/null audio backend.
- Positional 3D audio, attenuation, Doppler effects, reflections, and reverb-related processing.
- Vehicle, pedestrian, collision, weapon, weather, scripted, and frontend sounds.
- Radio stations, station announcements, and police radio.
- Frontend music, cutscene music, and ambient tracks.
- Custom MP3 radio playback.
- MP3 decoding through mpg123.
- WAV decoding through the internal decoder or optional libsndfile.
- Optional Opus support for audio streams and sound effects.
- Audio caching and optional multithreaded stream processing.
- Increased PS2-style audio-channel capacity when `PS2_AUDIO_CHANNELS` is enabled.
- Radio scrolling to the previous station when `RADIO_SCROLL_TO_PREV_STATION` is enabled.
- Radio pausing while the frontend is open when `PAUSE_RADIO_IN_FRONTEND` is enabled.
- Sounds that follow moving entities when `ATTACH_RELEASING_SOUNDS_TO_ENTITIES` is enabled.
- Audio speed adjustment tied to game time scale when `USE_TIME_SCALE_FOR_AUDIO` is enabled.
- Music/SFX volume, speaker configuration, audio-provider selection, and dynamic-acoustics settings.

Main sources: `src/audio/`

## 7. Controls and user interface

- Keyboard and mouse controls with remapping.
- Mouse sensitivity, vertical inversion, mouse steering, and head-bob settings.
- Rotatable/free camera when `FREE_CAM` is enabled.
- Improved debug camera when `IMPROVED_CAMERA` is enabled.
- Windows XInput controller support where the build configuration enables `XINPUT`.
- GLFW joystick/gamepad support and controller-mapping database.
- Controller-detection menu where `DETECT_JOYSTICK_MENU` is enabled.
- Controller vibration support, platform-dependent.
- Automatic switching between keyboard/mouse and controller prompts when `DETECT_PAD_INPUT_SWITCH` is enabled.
- PlayStation, Xbox, and Switch controller-button textures.
- Rebindable vehicle-fire action when `BIND_VEHICLE_FIREWEAPON` is enabled.
- Controller look inversion when `INVERT_LOOK_FOR_PAD` is enabled.
- Pause, new-game, load, delete, options, statistics, and mission-brief screens.
- Fullscreen frontend map with waypoint placement when `MENU_MAP` is enabled and map textures are present.
- Separate graphics settings and expanded display options when `GRAPHICS_MENU_OPTIONS` is enabled.
- Pedestrian- and traffic-density sliders when `PED_CAR_DENSITY_SLIDERS` is enabled.
- Scrollable statistics page when `SCROLLABLE_STATS_PAGE` is enabled.
- HUD, radar, blips, area names, messages, pager text, help text, timers, and wanted-level display.
- Height-indicating triangular radar blips when `TRIANGULAR_BLIPS` is enabled.
- Optional radio-off and radio-station text.
- Subtitles and optional cutscene borders.
- Player-skin selection.

Main sources: `src/core/Frontend.cpp`, `src/core/MenuScreensCustom.cpp`, `src/core/Pad.cpp`, `src/core/ControllerConfig.cpp`, `src/renderer/Hud.cpp`, `src/core/Radar.cpp`

## 8. Saves, settings, and localization

- Eight normal save slots.
- Save loading, deletion, overwrite confirmation, timestamps, and corruption checks.
- Portable save formatting when `COMPATIBLE_SAVES` is enabled.
- Optional repair/conversion of incompatible saves when `FIX_INCOMPATIBLE_SAVES` is enabled.
- Mission-retry functionality using quicksave data when `MISSION_REPLAY` is enabled.
- Settings stored in `re3.ini` when `LOAD_INI_SETTINGS` is enabled.
- User files stored in the game directory by default, as documented in the README.
- Persistent video-mode, controller, audio, graphics, language, and skin settings.
- English, French, German, Italian, and Spanish text.
- Additional Polish, Russian, and Japanese support when `MORE_LANGUAGES` is enabled and matching text/font files are present.
- GXT text loading and font rendering.
- Optional skipping of intro videos when `NO_MOVIES` is enabled.
- Audio-CD-check removal through `NO_CDCHECK`.
- Save-size validation when `VALIDATE_SAVE_SIZE` is enabled.

Main sources: `src/save/`, `src/text/`, `src/core/re3.cpp`, `gamefiles/TEXT/`

## 9. Replay, debugging, and cheats

- Gameplay recording and playback when `GTA_REPLAY` is enabled.
- Replay saving to and loading from `replay.rep`.
- Replay camera modes and playback controls.
- Ctrl+M debug menu when `DEBUGMENU` is enabled.
- Debug camera mode and debug-menu camera controls.
- Time, weather, wind, and time-scale controls.
- Vehicle spawning, repair, placement, collision toggling, and colour controls.
- Mission selection and mission switching.
- Teleportation to map waypoints.
- Rendering-category toggles and path/collision/cull-zone visualization.
- HUD, scaling, radar, sprite, frame-rate, and post-processing controls.
- Script debugging and optional script logs.
- Scene-editing code for actors, cameras, recording, playback, and movie save/load through `movie.dat`.
- Model/animation-viewer code.
- Debug console output and timing bars.
- Standard cheats for weapons, money, health, armour, wanted level, weather, time, vehicles, and pedestrian behaviour.
- Additional cheats:
  - Kangaroo/enhanced jumping cheat
  - All-cars-heli cheat
  - Alternate Dodo handling cheat
- Free-roam/debug script selection at startup:
  - `G`: normal `main.scm`
  - `R`: `main_freeroam.scm`
  - `D`: `main_d.scm`

Replay keys in the inspected source:

- `F1`: start playback while recording, or finish playback while playing.
- `F2`: save replay to disk while recording.
- `F3`: load replay from disk while recording.

Note: the README mentions Ctrl+B for the debug camera. The inspected source clearly exposes debug-camera control through the debug menu; the Ctrl+B shortcut was not independently verified in the current source.

Main sources: `src/control/Replay.cpp`, `src/control/SceneEdit.cpp`, `src/extras/debugmenu.cpp`, `src/core/re3.cpp`, `src/core/Pad.cpp`, `src/core/Cam.cpp`

## 10. Modding and repository utilities

- Loading of original game models, textures, collision files, animations, handling data, and scripts.
- Configurable entity, model, vehicle, pedestrian, and object limits in `src/core/config.h`.
- Compile-time feature switches in `src/core/config.h`.
- Documented asset-mod compatibility.
- CLEO compatibility through the external CLEO Redux project.
- Particle configuration and textures.
- Controller-mapping database.
- Additional fonts, menu textures, and controller-button textures.
- Neo rendering assets and tuning tables.
- Free-roam and debug SCM files.
- GXT compilation utility and editable translation text files.
- Git-revision embedding utilities.
- Visual Studio Code tasks for Linux/macOS premake and make builds.
- Issue and pull-request templates.
- Coding-style and clang-format configuration.

Main sources: `gamefiles/`, `utils/gxt/`, `src/core/config.h`, `README.md`, `.vscode/`, `.github/`

## 11. Platform and build support

- Documented support for Windows, Linux, macOS, and FreeBSD.
- x86, x86-64, ARM, and ARM64 build configurations.
- Nintendo Switch build support and CI workflow.
- Separate external ports referenced for PlayStation Vita and Wii U.
- CMake build system.
- Premake projects for Visual Studio 2015, 2017, and 2019.
- Conan dependency/build integration.
- CodeWarrior 7 reference project.
- GitHub Actions build and packaging workflows.
- CPack ZIP packaging and optional game-file installation.
- Optional link-time optimization.
- AddressSanitizer and undefined-behaviour sanitizer options.
- Optional Git-hash/version embedding.
- Optional post-build copying into a GTA III installation.
- Vendored or external librw selection.
- Selectable audio backend and optional codec dependencies.

Main sources: `CMakeLists.txt`, `premake5.lua`, `conanfile.py`, `.github/workflows/`, `codewarrior/`

## 12. Engine internals and resource management

- Entity hierarchy: placeable objects, entities, physical entities, buildings, treadables, dummies, objects, pedestrians, and vehicles.
- Object pools for pedestrians, vehicles, buildings, treadables, objects, dummies, pointer nodes, entry-info nodes, and audio script objects.
- Handle/reference management for saved and scripted entities.
- Spatial sectors, pointer lists, entry-info lists, and moving-entity lists.
- Line-of-sight, vertical-line, sphere, and range queries against the world.
- Zone hierarchy, map zones, audio zones, navigation zones, and cull zones.
- Attribute zones for gameplay/rendering restrictions.
- Streaming channels, priority requests, model/TXD loading, and unused-resource removal.
- IMG directory loading and additional CD-image support.
- Case-insensitive/file-manager helpers and cross-platform file access.
- Memory heap and memory-manager code, including optional custom allocator paths.
- Model-info categories:
  - simple models
  - time models
  - MLO models
  - clump models
  - pedestrian models
  - vehicle models
  - extra-component models
- Texture dictionary store and texture pools.
- RenderWare compatibility layer through `src/fakerw/` when using librw.
- Visibility plugins and alpha-entity sorting.
- Math helpers for vectors, matrices, quaternions, rectangles, and general math.
- Collision models, collision points, compressed vectors, and temporary collision models.
- Surface table and treadable behaviour.
- Game clock, timers, time steps, pause state, and slow-motion state.
- Event list and crime reporting hooks.
- Credits, console output, debug text, sprites, fonts, HUD, radar, messages, and pager systems.
- Statistics and player-info persistence.

Main sources: `src/core/`, `src/entities/`, `src/buildings/`, `src/collision/`, `src/modelinfo/`, `src/rw/`, `src/math/`, `src/fakerw/`

## 13. Animation and cutscene systems

- IFP animation loading.
- Animation blocks, hierarchies, sequences, nodes, associations, and association groups.
- Animation blending, partial animations, movement animations, and translation handling.
- Animation synchronization and finish/delete callbacks.
- Optional animation compression/uncompression paths.
- Bone-tag mapping and skinned/non-skinned frame updates.
- Cutscene data loading, cutscene objects, cutscene heads, and cutscene animation assignment.
- Cutscene timing, offsets, LOD multiplier handling, and cleanup.
- Camera curves and spline/path helpers used by control and camera systems.
- Replay interpolation of pedestrian and vehicle animation state.

Main sources: `src/animation/`, `src/control/Curves.cpp`, `src/control/Replay.cpp`

## 14. Scripting and mission-content support

- SCM script space, running scripts, idle/active script lists, mission cleanup, and script variables.
- Over 1,100 script-command registrations in the inspected source.
- Script commands for:
  - players, pedestrians, vehicles, objects, and coordinates
  - missions, timers, conditions, loops, and branches
  - text, HUD, radar, blips, markers, and messages
  - audio, radio, music, and cutscenes
  - weapons, pickups, garages, cranes, phones, and restart points
  - weather, clock, wanted level, gangs, zones, and statistics
  - debugging, logging, and script measurement conversion
- Optional metric script measurements when `USE_MEASUREMENTS_IN_METERS` is enabled.
- More precise foot/meter conversion when `USE_PRECISE_MEASUREMENT_CONVERTION` is enabled.
- Basic or advanced script debug output, depending on configuration.
- Optional script log levels through `SCRIPT_LOG_FILE_LEVEL`.
- Debug mission switcher roster containing 80 entries, including intro/info scenes, side activities, and story missions.

Main sources: `src/control/Script*.cpp`, `src/control/ScriptCommands.h`, `src/control/ScriptDebug.cpp`, `src/core/re3.cpp`

## 15. Configuration switches

The main feature configuration lives in `src/core/config.h`.

### Active or commonly enabled in the non-vanilla PC configuration

- `GTA_PC`
- `GTA_VERSION = GTA3_PC_11`
- `NO_CDCHECK`
- `DEFAULT_NATIVE_RESOLUTION`
- `GTA_PS2_STUFF`
- `NASTY_GAME`
- `DRAW_GAME_VERSION_TEXT`, with runtime display disabled by default
- `VALIDATE_SAVE_SIZE`
- `DEBUGMENU`
- `MISSION_SWITCHER`, when debug menu is enabled
- `FIX_BUGS`
- `FIX_BUGS_64`, on 64-bit builds
- `MORE_LANGUAGES`
- `COMPATIBLE_SAVES`
- `FIX_INCOMPATIBLE_SAVES`
- `LOAD_INI_SETTINGS`
- `NO_MOVIES`
- `EXTENDED_OFFSCREEN_DESPAWN_RANGE`
- `ASCII_STRCMP`, on applicable Windows builds
- `ASPECT_RATIO_SCALE`
- `PROPER_SCALING`
- `USE_TXD_CDIMAGE`
- `PS2_ALPHA_TEST`
- `IMPROVED_VIDEOMODE`
- `DISABLE_LOADING_SCREEN`
- `DISABLE_VSYNC_ON_TEXTURE_CONVERSION`
- `ANISOTROPIC_FILTERING`
- `FIX_SPRITES`
- `DETECT_PAD_INPUT_SWITCH`
- `KANGAROO_CHEAT`
- `ALLCARSHELI_CHEAT`
- `ALT_DODO_CHEAT`
- `REGISTER_START_BUTTON`
- `BIND_VEHICLE_FIREWEAPON`
- `BUTTON_ICONS`
- `HUD_ENHANCEMENTS`
- `TRIANGULAR_BLIPS`
- `FIX_RADAR`
- `RADIO_OFF_TEXT`
- `PC_MENU`
- `SCROLLABLE_STATS_PAGE`
- `TRIANGLE_BACK_BUTTON`
- `CUSTOM_FRONTEND_OPTIONS`
- `USE_DEBUG_SCRIPT_LOADER`
- `USE_MEASUREMENTS_IN_METERS`
- `USE_PRECISE_MEASUREMENT_CONVERTION`
- `USE_BASIC_SCRIPT_DEBUG_OUTPUT`, unless advanced logging is enabled
- `EXPLODING_AIRTRAIN`
- `CAMERA_PICKUP`
- `PED_SKIN`, requiring compatible saves
- `ANIMATE_PED_COL_MODEL`
- `CANCELLABLE_CAR_ENTER`
- `IMPROVED_CAMERA`
- `FREE_CAM`
- `EXTERNAL_3D_SOUND`
- `AUDIO_REFLECTIONS`
- `RADIO_SCROLL_TO_PREV_STATION`
- `AUDIO_CACHE`
- `PS2_AUDIO_CHANNELS`
- `PS2_AUDIO_PATHS`
- `AUDIO_OAL_USE_MPG123`
- `PAUSE_RADIO_IN_FRONTEND`
- `ATTACH_RELEASING_SOUNDS_TO_ENTITIES`
- `USE_TIME_SCALE_FOR_AUDIO`
- `MULTITHREADED_AUDIO`
- `BIG_IMG`, explicitly marked incomplete

### Conditional on backend, platform, or build options

- `XINPUT`: Windows builds not using `RW_GL3`.
- `DETECT_JOYSTICK_MENU`: XInput or applicable GLFW builds.
- `GAMEPAD_MENU`: XInput or handheld builds.
- `MENU_MAP`, `GRAPHICS_MENU_OPTIONS`, `NO_ISLAND_LOADING`, `CUTSCENE_BORDERS_SWITCH`, `MULTISAMPLING`, `INVERT_LOOK_FOR_PAD`, `PED_CAR_DENSITY_SLIDERS`: under `CUSTOM_FRONTEND_OPTIONS`.
- `MISSION_REPLAY`: under `PC_MENU`.
- `EXTENDED_COLOURFILTER`, `EXTENDED_PIPELINES`, `SCREEN_DROPLETS`, `NEW_RENDERER`: under `LIBRW`.
- `SCREEN_DROPLETS`: also requires `EXTENDED_COLOURFILTER`.
- `AUDIO_OAL_USE_OPUS`, `OPUS_AUDIO_PATHS`, `OPUS_SFX`: under `AUDIO_OPUS`.
- `FLUSHABLE_STREAMING`: non-Windows, non-Switch builds.
- `GTA_HANDHELD`: Switch or PSP2 builds.
- `IGNORE_MOUSE_KEYBOARD`: handheld builds.
- `USE_UNNAMED_SEM`: Switch builds.
- `USE_PS2_RAND`, `RANDOMSPLASH`, `PS2_MATFX`: PC builds with `GTA_PS2_STUFF`; `RANDOMSPLASH` is undefined when `DISABLE_LOADING_SCREEN` is enabled.
- `AUDIO_OAL_USE_SNDFILE`: optional build choice.
- `VANILLA_DEFINES`: CodeWarrior or vanilla build configuration.
- `FINAL` / `MASTER`: release-style configurations; not the default inspected configuration.
- `USE_MY_DOCUMENTS`: under `FINAL`.
- `CHATTYSPLASH` / `TIMEBARS`: non-`FINAL` configurations.

### Present but disabled, optional, or experimental

- `GTA_PS2` / `GTA_XBOX` target selections.
- `USE_CUSTOM_ALLOCATOR`, marked unfinished.
- `COMPRESSED_COL_VECTORS`.
- `ANIM_COMPRESSION`.
- `EXTRA_MODEL_FLAGS` / `HARDCODED_MODEL_FLAGS`.
- `USE_TEXTURE_POOL`.
- `PC_PARTICLE`.
- `PS2_ALTERNATIVE_CARSPLASH`, marked unused on PS2.
- `PS2_HUD`.
- `BETA_SLIDING_TEXT`.
- `XBOX_SUBTITLES`.
- `PS2_MENU`, `PS2_MENU_USEALLPAGEICONS`, `PS2_LIKE_MENU`, `PS2_SAVE_DIALOG`, `CIRCLE_BACK_BUTTON`.
- `SIMPLIER_MISSIONS`.
- `DONT_FIX_REPLAY_BUGS`.
- `USE_BETA_REPLAY_MODE`, marked buggy.
- `REMOVE_TREADABLE_PATHFIND`.
- `MONEY_MESSAGES`.
- `VC_PED_PORTS`.
- `NEW_WALK_AROUND_ALGORITHM`.
- `PEDS_REPORT_CRIMES_ON_PHONE`.
- `PS2_CAM_TRANSITION`.
- `ONE_THREAD_PER_CHANNEL`.
- `SQUEEZE_PERFORMANCE`, which changes several other feature defaults.
- `VU_COLLISION`, associated with PS2-specific collision code.

## 16. Build matrix and developer tooling

### Premake configurations

- Windows x86:
  - RenderWare 3.3 / Direct3D 8 with MSS or OpenAL
  - librw / Direct3D 9 with MSS or OpenAL
  - librw / OpenGL / GLFW with MSS or OpenAL
- Windows x64:
  - librw / Direct3D 9 with OpenAL
  - librw / OpenGL / GLFW with OpenAL
- Linux x86, x64, ARM, ARM64:
  - librw / OpenGL / GLFW / OpenAL
- FreeBSD x86, x64, ARM, ARM64:
  - librw / OpenGL / GLFW / OpenAL
- macOS x64 and ARM64:
  - librw / OpenGL / GLFW / OpenAL
- Debug and Release configurations.
- Vanilla configuration for Windows x86-oriented original-style builds.

### CMake options

- `RE3_AUDIO`: OpenAL, or MSS on Windows.
- `RE3_INSTALL`: install executable and game files.
- `RE3_WITH_OPUS`: optional Opus support.
- `RE3_WITH_LIBSNDFILE`: optional libsndfile instead of internal WAV decoding.
- `RE3_VENDORED_LIBRW`: use vendored librw or find an external package.
- `RE3_WITH_SANITIZERS`: undefined-behaviour sanitizer options.
- `RE3_WITH_ASAN`: AddressSanitizer.
- Nintendo Switch target support through devkitA64 CMake helpers.
- CPack ZIP packaging.

### Premake options

- `--glfwdir32`
- `--glfwdir64`
- `--with-asan`
- `--with-librw`
- `--with-opus`
- `--with-lto`
- `--no-git-hash`
- `--no-full-paths`

### Environment helpers

- `GTA_III_RE_DIR`: optional post-build copy/debug directory.
- `LIBRW`: optional external librw path.

### Active CI workflows inspected

- Windows x86 premake builds:
  - librw D3D9 MSS
  - librw GL3 GLFW MSS
  - librw D3D9 OpenAL
  - librw GL3 GLFW OpenAL
  - Debug, Release, and Vanilla build types
- Windows x64 premake builds:
  - librw D3D9 OpenAL
  - librw GL3 GLFW OpenAL
  - Debug and Release build types
- CMake/Conan builds:
  - Windows GL3/OpenAL
  - Windows D3D9/OpenAL
  - Ubuntu 18.04 GL3/OpenAL
  - macOS GL3/OpenAL
- Nintendo Switch devkitA64 CMake build and CPack artifact.

Note: the workflows use older GitHub-hosted runners and action versions. Their presence does not prove they currently pass without maintenance.

### Other developer tools

- Visual Studio 2015/2017/2019 premake launcher scripts.
- Linux premake binary.
- Windows premake binary.
- Git-hash generation scripts for Windows and Unix-like systems.
- CodeWarrior project XML and debug/release link-order files.
- VS Code C/C++ properties, launch configuration, settings, and build tasks.
- CMake find modules for Miles SDK, libsndfile, mpg123, and opusfile.
- CMake Git-revision description helper.
- Nintendo Switch CMake helper functions.
- Conan recipe for dependency/build integration.
- GXT build script and Windows GXT executable.
- clang-format configuration and coding-style document.
- GitHub issue and pull-request templates.

## 17. Included supplemental data and utilities

This repository does **not** include a complete GTA III installation. It includes supplemental files used by re3:

### Text and fonts

- `gamefiles/TEXT/american.gxt`
- `gamefiles/TEXT/english.gxt`
- `gamefiles/TEXT/french.gxt`
- `gamefiles/TEXT/german.gxt`
- `gamefiles/TEXT/italian.gxt`
- `gamefiles/TEXT/spanish.gxt`
- `gamefiles/TEXT/polish.gxt`
- `gamefiles/TEXT/russian.gxt`
- `gamefiles/TEXT/JAPANESE.gxt`
- `gamefiles/models/fonts_p.txd`
- `gamefiles/models/fonts_r.txd`
- `gamefiles/models/fonts_j.txd`

### Scripts and gameplay data

- `gamefiles/data/main_freeroam.scm`
- `gamefiles/data/main_d.scm`
- `gamefiles/data/PARTICLE.CFG`

### UI and controller assets

- `gamefiles/gamecontrollerdb.txt`
- `gamefiles/models/frontend_ds3.txd`
- `gamefiles/models/frontend_ds4.txd`
- `gamefiles/models/frontend_nsw.txd`
- `gamefiles/models/frontend_x360.txd`
- `gamefiles/models/frontend_xone.txd`
- `gamefiles/models/generic.txd`
- `gamefiles/models/menu.txd`
- `gamefiles/models/nswbtns.txd`
- `gamefiles/models/particle.txd`
- `gamefiles/models/ps3btns.txd`
- `gamefiles/models/x360btns.txd`

### Neo rendering assets

- `gamefiles/neo/neo.txd`
- `gamefiles/neo/carTweakingTable.dat`
- `gamefiles/neo/rimTweakingTable.dat`
- `gamefiles/neo/worldTweakingTable.dat`

### GXT utilities

- `utils/gxt/gxt.exe`
- `utils/gxt/build.bat`
- Editable text files for American English, English, French, German, Italian, Polish, Russian, and Spanish.

### Vendored dependencies

Tracked Windows import libraries/DLLs and headers include:

- libsndfile
- Miles SDK import library/header
- mpg123
- OpenAL Soft

Git submodules are declared for:

- librw
- Ogg
- Opus
- Opusfile

In the inspected checkout, those four submodules were uninitialized.

## 18. Source directory map

| Directory | Main responsibility |
|---|---|
| `src/animation/` | Animation blending, IFP loading, bones, cutscenes |
| `src/audio/` | Audio manager, music, radio, OpenAL, Miles, EAX-related helpers |
| `src/buildings/` | Buildings, solids, treadables |
| `src/collision/` | Collision models, shapes, queries, PS2/VU collision code |
| `src/control/` | Scripts, game logic, AI, garages, paths, pickups, replay, scene edit |
| `src/core/` | Main game loop, frontend, camera, streaming, world, input, config, debug |
| `src/entities/` | Entity and physical-entity base classes |
| `src/extras/` | Debug menu, frontend options, custom pipelines, postfx, droplets, INI |
| `src/fakerw/` | RenderWare-compatible API surface for librw |
| `src/math/` | Vectors, matrices, quaternions, rectangles, math helpers |
| `src/modelinfo/` | Model metadata for simple, time, MLO, clump, ped, vehicle models |
| `src/objects/` | Objects, cutscene objects, particle objects, projectiles |
| `src/peds/` | Player, civilians, cops, emergency peds, gangs, population, ped AI |
| `src/renderer/` | HUD, fonts, sprites, weather, water, shadows, particles, effects |
| `src/rw/` | RenderWare helpers, textures, lights, memory, visibility, MatFX |
| `src/save/` | PC saves, generic storage, save buffers, memory-card code |
| `src/skel/` | Platform skeleton, events, GLFW and Windows backend code |
| `src/text/` | GXT text, messages, pager |
| `src/vehicles/` | Cars, boats, trains, planes, helicopters, cranes, handling, damage |
| `src/weapons/` | Weapons, bullets, shots, projectiles, explosions, weapon effects |

## 19. Limitations, exclusions, and caveats

- Original GTA III PC game assets are required.
- The repository does not include the full original game maps, models, audio, or main mission script.
- librw, Ogg, Opus, and Opusfile submodules were uninitialized in the inspected checkout.
- Vanilla RenderWare builds require original RenderWare libraries and DirectX 8 SDK components.
- Miles builds require original Miles DLLs from the game.
- PS2 and Xbox builds are not currently supported as working targets.
- High-FPS physics fixes remain on the project to-do list.
- Low-end performance, especially OpenGL on Raspberry Pi, is listed as needing improvement.
- Larger IMG-archive support is marked incomplete.
- Custom allocator, animation compression, and compressed collision vectors are optional/experimental.
- Alternative replay slow-motion mode is explicitly marked buggy.
- Multiplayer menu entries are placeholders; this is a single-player implementation.
- Native DLL/ASI mods and limit adjusters are not generally compatible.
- CLEO support depends on the external CLEO Redux project.
- Some features require extra assets:
  - menu map requires compatible `menu.txd`
  - Neo options require `neo/neo.txd`
  - extra languages require matching GXT and font files
  - skinned pedestrians require compatible skinned models
  - Xbox world lightmaps require compatible Xbox map assets
  - PS2 audio paths may require `vbdec` on MSS builds
  - Opus support requires an Opus-enabled build and compatible audio files
- No dedicated automated gameplay/unit test suite was found during inspection; CI is build/package oriented.
- The project does not provide a standard open-source license. The README states educational, documentation, and modding use, discourages piracy/commercial use, and asks derivative work to remain open and credited.

## Appendix A. Debug mission-switcher roster

These names come from the debug mission switcher in `src/core/re3.cpp`. They are switcher entries, not proof that all required assets are included in this repository.

1. Intro Movie
2. Hospital Info Scene
3. Police Station Info Scene
4. RC Diablo Destruction
5. RC Mafia Massacre
6. RC Rumpo Rampage
7. RC Casino Calamity
8. Patriot Playground
9. A Ride In The Park
10. Gripped!
11. Multistorey Mayhem
12. Paramedic
13. Firefighter
14. Vigilante
15. Taxi Driver
16. The Crook
17. The Thieves
18. The Wife
19. Her Lover
20. Give Me Liberty and Luigi's Girls
21. Don't Spank My Bitch Up
22. Drive Misty For Me
23. Pump-Action Pimp
24. The Fuzz Ball
25. Mike Lips Last Lunch
26. Farewell 'Chunky' Lee Chong
27. Van Heist
28. Cipriani's Chauffeur
29. Dead Skunk In The Trunk
30. The Getaway
31. Taking Out The Laundry
32. The Pick-Up
33. Salvatore's Called A Meeting
34. Triads And Tribulations
35. Blow Fish
36. Chaperone
37. Cutting The Grass
38. Bomb Da Base: Act I
39. Bomb Da Base: Act II
40. Last Requests
41. Turismo
42. I Scream, You Scream
43. Trial By Fire
44. Big'N'Veiny
45. Sayonara Salvatore
46. Under Surveillance
47. Paparazzi Purge
48. Payday For Ray
49. Two-Faced Tanner
50. Kanbu Bust-Out
51. Grand Theft Auto
52. Deal Steal
53. Shima
54. Smack Down
55. Silence The Sneak
56. Arms Shortage
57. Evidence Dash
58. Gone Fishing
59. Plaster Blaster
60. Marked Man
61. Liberator
62. Waka-Gashira Wipeout!
63. A Drop In The Ocean
64. Bling-Bling Scramble
65. Uzi Rider
66. Gangcar Round-Up
67. Kingdom Come
68. Grand Theft Aero
69. Escort Service
70. Decoy
71. Love's Disappearance
72. Bait
73. Espresso-2-Go!
74. S.A.M.
75. Uzi Money
76. Toyminator
77. Rigged To Blow
78. Bullion Run
79. Rumble
80. The Exchange

## Appendix B. Verification status

- Inspected: documentation, configuration, source layout, subsystem interfaces, build files, CI workflows, included data files, and submodule state.
- Not performed: compilation, linking, asset validation, gameplay testing, controller testing, audio testing, save compatibility testing, or platform-specific runtime testing.
- Therefore, this inventory describes features present in the repository and its configuration, not a guarantee that every option works in every build.
