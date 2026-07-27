# timestamp

Returns the last modified date of all the files in a Linux directory.

Recursively walks the specified directory (or current directory) and prints the most recent `last_write_time` across all files.

## Usage

```bash
timestamp [OPTIONS] [search_dir1] [search_dir2]
```

| Option | Description |
|--------|-------------|
| `-h`   | Display help |
| `-e <regex>` | Exclude files matching this regex |
| `-v`   | Verbose output (to stderr) |

### Examples

```bash
timestamp                    # current dir
timestamp /some/path         # specific dir
timestamp -v /some/path      # verbose
timestamp -e '\.git' /some   # exclude pattern
```

## Dependencies

- C++23 compiler (GCC 14+, Clang 18+)
- Boost (program_options, regex)
- CMake 3.30+

## Build

```bash
# Configure (generates build system in the build/ directory)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build (compiles the binary: build/timestamp)
cmake --build build

# Build (release — optimised, no debug symbols)
cmake --build build --config Release

# Install (copies the binary to PREFIX/bin/timestamp; default PREFIX=/usr/local)
cmake --install build

# Install locally (PREFIX = project root → ./bin/timestamp)
cmake --install build --prefix .
```

## Test

```bash
# Run smoke tests with CTest
ctest --test-dir build --output-on-failure
```

`--output-on-failure` prints test output only when a test fails; on success it stays silent.

### Manual smoke test

```bash
./build/timestamp                    # current dir (expected: exits 0, prints a date)
./build/timestamp --help             # expected: exits 0, prints usage
./build/timestamp /nonexistent       # expected: exits 1, error on stderr
./build/timestamp -v src/            # expected: verbose output on stderr
./build/timestamp -e '\.cpp' src/    # expected: filters out .cpp files
```

### Manual build (without CMake)

```bash
g++ -o timestamp.o -c -std=c++23 -I/path/to/boost src/timestamp.cpp
g++ -o timestamp timestamp.o -L/path/to/boost/stage/lib -lboost_regex -lboost_program_options
```

## Lint / static analysis

```bash
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
run-clang-tidy -p build
```

## Format

The project uses Google style (4-space indent, 120 column limit), defined in `.clang-format`.

The CI `format` job runs `clang-format --dry-run --Werror` and fails if any file doesn't match. Check formatting locally with:

```bash
# Check (exit 0 = clean, exit 1 = changes needed)
clang-format --dry-run --Werror src/timestamp.cpp

# Auto-format in-place
clang-format -i src/timestamp.cpp

# Format all source files
find src/ -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i

# Format only lines changed vs. HEAD (git clang-format)
git clang-format            # unstaged changes
git clang-format --staged   # staged changes, before commit
```

## License

CC0 1.0 Universal — do with it as you like.
