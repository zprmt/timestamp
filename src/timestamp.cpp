/*
 * timestamp.cpp
 *
 * Licensed under CC0 1.0 Universal — do with it as you like.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <filesystem>
#include <format>
#include <iostream>
// Is actually needed for file_time_type formatting, but clangd doesn't see it
#include <chrono>  // IWYU pragma: keep
#include <span>
#include <vector>

/*
 * This program recursively looks at all files in the current or supplied
 * directory, and prints the last modification across all files.
 */

namespace fs = std::filesystem;
namespace po = boost::program_options;

struct Config {
    std::vector<std::string> directories;
    boost::regex exclude_regex;
    bool use_regex = false;
    bool verbose = false;
};

Config parse_arguments(std::span<char*> args) {
    Config config;
    std::string exclude;

    po::options_description desc(
        R"(timestamp [OPTIONS] [search_dir1] [search_dir2]

This program recursively looks at all files in the current or supplied directory, and prints the last modification across all files

Options)");
    desc.add_options()("help,h", "Display this help")("exclude,e", po::value<std::string>(&exclude),
                                                      "Exclude files matching this regex")(
        "verbose,v", po::bool_switch(&config.verbose), "Verbose output to stderr");

    po::options_description hidden("Hidden");
    hidden.add_options()("search-dirs", po::value<std::vector<std::string>>(), "search-dirs");

    po::positional_options_description pos;
    pos.add("search-dirs", -1);

    po::options_description all;
    all.add(desc).add(hidden);

    po::variables_map var_map;

    po::store(po::command_line_parser(static_cast<int>(args.size()), args.data()).options(all).positional(pos).run(),
              var_map);

    // std::map::contains() is C++20, and MSVC marks it dllimport but doesn't
    // export it from its runtime DLL (or the installed version on the runner
    // doesn't have it).
    //
    // After further research done by Claude: this isn't actually about a
    // missing/outdated runtime DLL. boost::program_options::variables_map is
    // declared dllimport because of BOOST_PROGRAM_OPTIONS_DYN_LINK, and it
    // derives from std::map<...>. MSVC propagates a class's dllimport
    // attribute onto its inherited members too, so the compiler tries to
    // import std::map::contains() from boost_program_options.dll instead of
    // compiling it locally like a normal header-only STL function. Boost's
    // DLL never exported that symbol (it's not Boost's to export), so the
    // link always fails here, regardless of vcpkg/MSVC version.
    //
    // So this will fix itself when we move to statically linking boost.
    // NOLINTNEXTLINE(readability-container-contains)
    if (var_map.count("help") != 0) {
        std::cout << desc;
        std::exit(0);
    }

    po::notify(var_map);

    // See comment above
    // NOLINTNEXTLINE(readability-container-contains)
    if (var_map.count("search-dirs") == 0) {
        config.directories = {"."};
    } else {
        config.directories = var_map["search-dirs"].as<std::vector<std::string>>();
    }

    if (!exclude.empty()) {
        config.use_regex = true;
        config.exclude_regex = boost::regex(exclude);
    }

    return config;
}

std::filesystem::file_time_type calculate_timestamp(const Config& config) {
    std::filesystem::file_time_type timestamp = std::filesystem::file_time_type::min();
    for (const auto& dir : config.directories) {
        for (const fs::directory_entry& dir_entry :
             fs::recursive_directory_iterator(dir, fs::directory_options::skip_permission_denied)) {
            auto file_timestamp = dir_entry.last_write_time();
            if (config.use_regex && boost::regex_search(dir_entry.path().string(), config.exclude_regex)) {
                continue;
            }
            if (file_timestamp > timestamp) {
                timestamp = file_timestamp;
            }
            if (config.verbose) {
                std::cerr << dir_entry.path() << " " << std::format("{}", dir_entry.last_write_time()) << '\n';
            }
        }
    }
    return timestamp;
}

int main(int argc, char* argv[]) {
    try {
        Config config = parse_arguments(std::span<char*>(argv, static_cast<std::size_t>(argc)));
        auto timestamp = calculate_timestamp(config);
        std::cout << std::format("{}", timestamp) << '\n';
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}