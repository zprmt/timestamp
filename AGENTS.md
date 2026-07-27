# AGENTS.md — learning project context

This is a learning project. The primary goals are:

1. **Learn CMake** — write idiomatic `CMakeLists.txt` from scratch, handle external dependencies (Boost), configure install targets, and produce a clean build system.

2. **Learn GitHub Actions** — set up CI workflows that build the project across platforms, run smoke tests, and possibly lint/format check.

3. **Practice modern C++** — use C++23 features (`std::filesystem`, `std::format`) alongside real Boost libraries.

The code itself (`src/timestamp.cpp`) is a deliberately small, single-file tool so that build-system and CI concerns are the focus, not application complexity.

## Current state

- Single file `src/timestamp.cpp`
- `CMakeLists.txt` — done
- Single git commit
- No CI workflows

## Things to do

See [TODO.md](TODO.md)