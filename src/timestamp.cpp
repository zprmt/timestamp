/*
 * timestamp.cpp — reference implementation
 *
 * Licensed under CC0 1.0 Universal — do with it as you like.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <iostream>
#include <filesystem>
#include <format>
// Is actually needed for file_time_type formatting, but clang-tidy doesn't see it
// NOLINTNEXTLINE(unused-include)
#include <chrono>
#include <boost/program_options.hpp>
#include <vector>
#include <boost/regex.hpp>

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

Config parse_arguments(int argc, char* argv[]) {
    Config config;
    std::string exclude;

    po::options_description desc(
R"(timestamp [OPTIONS] [search_dir1] [search_dir2]

This program recursively looks at all files in the current or supplied directory, and prints the last modification across all files

Options)");
    desc.add_options()
        ("help,h", "Display this help")
        ("exclude,e", po::value<std::string>(&exclude), "Exclude files matching this regex")
        ("verbose,v", po::bool_switch(&config.verbose), "Verbose output to stderr")
    ;

    po::options_description hidden("Hidden");
    hidden.add_options()
        ("search-dirs", po::value<std::vector<std::string>>(), "search-dirs");

    po::positional_options_description pos;
    pos.add("search-dirs", -1);

    po::options_description all;
    all.add(desc).add(hidden);

    po::variables_map var_map;

    po::store(po::command_line_parser(argc, argv)
        .options(all)
        .positional(pos)
        .run(), var_map);

    if (var_map.contains("help")) {
        std::cout << desc;
        std::exit(0);
    }

    po::notify(var_map);

    if (!var_map.contains("search-dirs")) {
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
                std::cerr << dir_entry.path() << " " << std::format( "{}", dir_entry.last_write_time()) << '\n';
            }
        }
    }
    return timestamp;
}

int main(int argc, char* argv[]) {
    try {
        Config config = parse_arguments(argc, argv);
        auto timestamp = calculate_timestamp(config);
        std::cout << std::format("{}", timestamp) << '\n';
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

// Build object file: 
// g++ -o timestamp.o -c -std=c++23 -I/path/to/boost timestamp.cpp

// Build executable:
// g++ -o timestamp timestamp.o -L/path/to/boost/stage/lib -lboost_regex -lboost_program_options

// Build executable with static linking:
// g++ -o timestamp timestamp.o /path/to/boost/stage/lib/libboost_regex.a /path/to/boost/stage/lib/libboost_program_options.a