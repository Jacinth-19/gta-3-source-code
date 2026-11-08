# Releasing

Waypoint Engine uses GitHub Actions for release builds.

## Desktop release

1. Create a tag such as `v0.1.0`.
2. Push the tag.
3. The `Release` workflow builds Windows, Linux, and macOS runtime packages.
4. The workflow publishes a GitHub release with the platform zip files.

Each package contains the runtime executable, sample project, blank template, docs, README, LICENSE, and NOTICE.

## Editor release

The Qt 6 editor workflow is manual until Qt packaging is stable.
It builds the editor on Windows, Linux, and macOS.

## Android

The Android workflow is currently a native compile check.
A full APK/AAB export template is a later milestone.
