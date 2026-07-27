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
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build

# Build (release)
cmake --build build --config Release

# Install
cmake --install build
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

```bash
find src/ -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i -style=file
```

## License

CC0 1.0 Universal — do with it as you like.
