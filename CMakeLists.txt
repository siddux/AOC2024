# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.27)

project ("AOC2024" LANGUAGES CXX)

add_library(aoc_tools INTERFACE)
set_target_properties(aoc_tools PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(aoc_tools INTERFACE cxx_std_23)
target_include_directories(aoc_tools INTERFACE "lib")

# Include sub-projects.
add_subdirectory ("day01")
add_subdirectory ("day02")
add_subdirectory ("day03")
add_subdirectory ("day04")
add_subdirectory ("day05")
add_subdirectory ("day06")
add_subdirectory ("day07")
add_subdirectory ("day08")
add_subdirectory ("day09")
add_subdirectory ("day10")
add_subdirectory ("day11")
