# TODO

- [x] Write `CMakeLists.txt` — find Boost packages, set C++23 standard, add executable target, install rules
- [x] Add `.clang-format` and `.clang-tidy` configs
- [x] Create `.github/workflows/build.yml` — build on Ubuntu, run smoke tests
- [x] Add `bin/` to `.gitignore`
- [x] Add smoke tests (shell script or CTest)
- [ ] Switch `timestamp.cpp` to Boost's own linkage macro (`BOOST_ALL_DYN_LINK` / `BOOST_PROGRAM_OPTIONS_DYN_LINK`) instead of our own `TIMESTAMP_USE_STATIC_BOOST` define, so the `count()` fallback stays correct regardless of how Boost gets linked
- [ ] CI: add a Linux-only matrix entry (or job) configuring with `-DTIMESTAMP_STATIC_BOOST=OFF` so the dynamic-Boost fallback path is actually compiled in CI
- [ ] Add an `actions/cache` step (or vcpkg's `x-gha` binary cache) to make repeat Windows runs near-instant
