#!/usr/bin/env python3
"""Waypoint Engine beginner helper.

Examples:
  python tools/wp.py doctor
  python tools/wp.py new MyGame
  python tools/wp.py build
  python tools/wp.py run MyGame
  python tools/wp.py test
  python tools/wp.py export MyGame --release
  python tools/wp.py deps status
"""
from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build" / "wp"
LOCK = ROOT / "external" / "dependencies.lock.json"


def tool_dirs() -> list[Path]:
    """Extra directories that may contain CMake/Ninja installed by pip."""
    dirs: list[Path] = []
    for candidate in (
        ROOT.parent / ".waypoint-tools" / "bin",
        Path.home() / "waypoint-tools" / "bin",
    ):
        if candidate.is_dir():
            dirs.append(candidate)
    cmake = shutil.which("cmake")
    if cmake:
        dirs.append(Path(cmake).resolve().parent)
    # De-duplicate while preserving order.
    seen: set[Path] = set()
    unique: list[Path] = []
    for directory in dirs:
        resolved = directory.resolve()
        if resolved not in seen:
            seen.add(resolved)
            unique.append(resolved)
    return unique


def find_tool(name: str) -> str | None:
    found = shutil.which(name)
    if found:
        return found
    exe = name + (".exe" if os.name == "nt" else "")
    for directory in tool_dirs():
        candidate = directory / exe
        if candidate.exists():
            return str(candidate)
    return None


def run(cmd: list[str], cwd: Path | None = None) -> int:
    print("+", " ".join(str(part) for part in cmd), flush=True)
    env = os.environ.copy()
    extra = os.pathsep.join(str(directory) for directory in tool_dirs())
    if extra:
        env["PATH"] = extra + os.pathsep + env.get("PATH", "")
    return subprocess.call([str(part) for part in cmd], cwd=cwd or ROOT, env=env)


def doctor() -> int:
    print("Waypoint Engine doctor")
    ok = True
    for tool in ("git", "python3", "ninja", "cmake", "ctest"):
        path = find_tool(tool)
        print(f"  {tool}: {path or 'MISSING'}")
        ok = ok and bool(path)
    print(f"  dependency lock: {'OK' if LOCK.exists() else 'MISSING'}")
    ok = ok and LOCK.exists()
    if not ok:
        print("\nInstall missing tools, then run this command again.")
        print("Tip: python -m pip install cmake ninja")
        return 1
    print("\nLooks good. Try: python tools/wp.py build")
    return 0


def new_project(name: str) -> int:
    target = Path(name)
    if not target.is_absolute():
        target = Path.cwd() / target
    target = target.resolve()
    if target.exists():
        print(f"Folder already exists: {target}")
        return 1
    template = ROOT / "templates" / "blank_project"
    shutil.copytree(template, target)
    manifest = target / "waypoint.project.json"
    data = json.loads(manifest.read_text())
    data["name"] = target.name
    manifest.write_text(json.dumps(data, indent=2) + "\n")
    print(f"Created {target}")
    print(f"Next: python tools/wp.py run {target}")
    return 0


def configure(build_type: str = "Debug", editor: bool = False) -> int:
    cmake = find_tool("cmake")
    if not cmake:
        print("cmake is missing. Run: python tools/wp.py doctor")
        return 1
    args = [
        cmake,
        "-S",
        str(ROOT),
        "-B",
        str(BUILD),
        "-G",
        "Ninja",
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-DWAYPOINT_BUILD_EDITOR={'ON' if editor else 'OFF'}",
        "-DWAYPOINT_BUILD_TESTS=ON",
    ]
    ninja = find_tool("ninja")
    if ninja:
        args.append(f"-DCMAKE_MAKE_PROGRAM={ninja}")
    if os.environ.get("WAYPOINT_HEADLESS_BUILD") == "1":
        args.append("-DWAYPOINT_HEADLESS_BUILD=ON")
        args.append("-DWAYPOINT_BUILD_RUNTIME=OFF")
        args.append("-DWAYPOINT_ENABLE_RENDER=OFF")
    return run(args)


def build(build_type: str = "Debug", editor: bool = False) -> int:
    code = configure(build_type, editor)
    if code != 0:
        return code
    cmake = find_tool("cmake")
    return run([cmake, "--build", str(BUILD), "--parallel"])


def run_project(project: str | None) -> int:
    exe = BUILD / "runtime" / ("waypoint.exe" if os.name == "nt" else "waypoint")
    if not exe.exists():
        print("Runtime is not built yet. Run: python tools/wp.py build")
        return 1
    project_dir = Path(project).resolve() if project else Path.cwd()
    return run([str(exe), str(project_dir)])


def test() -> int:
    cmake = find_tool("cmake")
    ctest = find_tool("ctest")
    if not cmake or not ctest:
        print("cmake/ctest is missing. Run: python tools/wp.py doctor")
        return 1
    code = configure()
    if code != 0:
        return code
    code = run([cmake, "--build", str(BUILD), "--parallel"])
    if code != 0:
        return code
    return run([ctest, "--test-dir", str(BUILD), "--output-on-failure"])


def export_project(project: str, release: bool = False) -> int:
    if os.environ.get("WAYPOINT_HEADLESS_BUILD") == "1":
        print("Export needs the windowed runtime. Unset WAYPOINT_HEADLESS_BUILD first.")
        return 1
    project_dir = Path(project).resolve()
    manifest = project_dir / "waypoint.project.json"
    if not manifest.exists():
        print(f"Project manifest not found: {manifest}")
        return 1
    code = build("Release" if release else "Debug", editor=False)
    if code != 0:
        return code
    exe = BUILD / "runtime" / ("waypoint.exe" if os.name == "nt" else "waypoint")
    if not exe.exists():
        print(f"Runtime executable not found: {exe}")
        return 1
    data = json.loads(manifest.read_text())
    name = str(data.get("name", project_dir.name)).replace(" ", "-")
    platform = "windows" if os.name == "nt" else ("macos" if sys.platform == "darwin" else "linux")
    output = ROOT / "dist" / f"{name}-{platform}"
    if output.exists():
        shutil.rmtree(output)
    output.mkdir(parents=True)
    shutil.copy2(exe, output / exe.name)
    shaders = BUILD / "runtime" / "shaders"
    if shaders.exists():
        shutil.copytree(shaders, output / "shaders")
    shutil.copytree(project_dir, output / "game")
    (output / "RUN.txt").write_text(
        "Run this game with:\n\n"
        f"  ./{exe.name} game\n\n"
        "On Windows, double-click run.bat or use waypoint.exe game.\n"
    )
    if os.name == "nt":
        (output / "run.bat").write_text("@echo off\r\nwaypoint.exe game\r\npause\r\n")
    else:
        (output / "run.sh").write_text("#!/bin/sh\n./waypoint game\n")
        (output / "run.sh").chmod(0o755)
    print(f"Exported desktop build to {output}")
    return 0


def load_lock() -> list[dict]:
    data = json.loads(LOCK.read_text())
    return data["dependencies"]


def git_output(args: list[str], cwd: Path) -> str | None:
    result = subprocess.run(["git", *args], cwd=cwd, text=True, capture_output=True)
    if result.returncode != 0:
        return None
    return result.stdout.strip()


def deps_status() -> int:
    if not LOCK.exists():
        print("Dependency lock is missing.")
        return 1
    ok = True
    for dep in load_lock():
        directory = ROOT / "external" / dep["name"]
        if not directory.is_dir():
            print(f"  {dep['name']}: missing (run: python tools/wp.py deps fetch)")
            ok = False
            continue
        head = git_output(["rev-parse", "HEAD"], directory)
        if head == dep["commit"]:
            print(f"  {dep['name']}: OK {dep['tag']}")
        else:
            print(f"  {dep['name']}: wrong revision {head or 'unknown'}")
            ok = False
    return 0 if ok else 1


def deps_fetch() -> int:
    if not LOCK.exists():
        print("Dependency lock is missing.")
        return 1
    git = find_tool("git")
    if not git:
        print("git is missing.")
        return 1
    for dep in load_lock():
        directory = ROOT / "external" / dep["name"]
        print(f"Fetching {dep['name']} {dep['tag']} ({dep['commit']})")
        directory.parent.mkdir(parents=True, exist_ok=True)
        if not (directory / ".git").exists():
            if directory.exists():
                print(f"  Refusing to overwrite non-git folder: {directory}")
                return 1
            code = run([git, "init", str(directory)])
            if code != 0:
                return code
            code = run([git, "remote", "add", "origin", dep["repo"]], cwd=directory)
            if code != 0:
                return code
        code = run([git, "fetch", "--depth", "1", "origin", dep["commit"]], cwd=directory)
        if code != 0:
            return code
        code = run([git, "checkout", "--detach", dep["commit"]], cwd=directory)
        if code != 0:
            return code
    print("Dependency cache is ready in external/.")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Waypoint Engine helper")
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("doctor")
    p = sub.add_parser("new")
    p.add_argument("name")
    p = sub.add_parser("build")
    p.add_argument("--release", action="store_true")
    p.add_argument("--editor", action="store_true")
    p = sub.add_parser("run")
    p.add_argument("project", nargs="?")
    sub.add_parser("test")
    p = sub.add_parser("export")
    p.add_argument("project")
    p.add_argument("--release", action="store_true")
    p = sub.add_parser("deps")
    p.add_argument("action", choices=["status", "fetch"])
    args = parser.parse_args()

    if args.command == "doctor":
        return doctor()
    if args.command == "new":
        return new_project(args.name)
    if args.command == "build":
        return build("Release" if args.release else "Debug", args.editor)
    if args.command == "run":
        return run_project(args.project)
    if args.command == "test":
        return test()
    if args.command == "export":
        return export_project(args.project, args.release)
    if args.command == "deps":
        return deps_status() if args.action == "status" else deps_fetch()
    return 1


if __name__ == "__main__":
    sys.exit(main())
