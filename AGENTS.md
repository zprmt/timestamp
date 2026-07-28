# AGENTS.md — learning project context

This is a learning project. The primary goals are:

1. **Learn CMake** — write idiomatic `CMakeLists.txt` from scratch, handle external dependencies (Boost), configure install targets, and produce a clean build system.

2. **Learn GitHub Actions** — set up CI workflows that build the project across platforms, run smoke tests, and possibly lint/format check.

3. **Practice modern C++** — use C++23 features (`std::filesystem`, `std::format`) alongside real Boost libraries.

4. **clang-tidy discipline** — prefer fixing issues in the code over adding blanket exclusions to `.clang-tidy`. When a rule legitimately cannot be followed (e.g. platform compatibility), use `// NOLINTNEXTLINE(...)` or `// NOLINTBEGIN/END(...)` comments at the specific site, with a comment explaining why.

Remember to explain any actions you do related to CMake and GitHub Actions to the user and teach them to them. Specifically when using working on complex aspects of CMake and GitHub Actions and introducing new CMake and GitHub Actions features.

The code itself (`src/timestamp.cpp`) is a deliberately small, single-file tool so that build-system and CI concerns are the focus, not application complexity.

## Current state

- Single file `src/timestamp.cpp`
- `CMakeLists.txt` — done
- Smoke tests in `cmake/tests/`
- Single git commit
- CI workflow in `.github/workflows/build.yml` — build + test on push/PR

## Things to do

See [TODO.md](TODO.md)