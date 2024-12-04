#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <filesystem>
#include <numeric>
#include <source_location>
#include <string>
#include <ranges>
#include <utility>
#include <vector>

#include "aoc_tools.hpp"

int count_word_occurrences(const std::vector<std::string>& grid, const std::string& word) {
    const int rows = grid.size();
    const int cols = grid[0].size();
    const int word_length = word.length();

    // Define all 8 directions as (dx, dy)
    constexpr std::array<std::pair<int, int>, 8> directions = {{
        {0, 1},   // Right
        {0, -1},  // Left
        {1, 0},   // Down
        {-1, 0},  // Up
        {1, 1},   // Diagonal Down-Right
        {-1, -1}, // Diagonal Up-Left
        {1, -1},  // Diagonal Down-Left
        {-1, 1}   // Diagonal Up-Right
    }};

    auto is_valid = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < rows && y < cols;
    };

    int total_combinations = rows * cols * directions.size();

    auto combinations = std::views::iota(0, total_combinations);

    int count = std::accumulate(
        combinations.begin(), combinations.end(), 0,
        [&](int total, int idx) {
            int pos_idx = idx / directions.size();
            int dir_idx = idx % directions.size();
            int x = pos_idx / cols;
            int y = pos_idx % cols;
            auto [dx, dy] = directions[dir_idx];

            // Check if the word matches starting from (x, y) in direction (dx, dy)
            bool matches = std::ranges::all_of(
                std::views::iota(0, word_length),
                [&](int i) {
                    int nx = x + i * dx;
                    int ny = y + i * dy;
                    return is_valid(nx, ny) && grid[nx][ny] == word[i];
                }
            );

            return total + (matches ? 1 : 0);
        }
    );

    return count;
}


int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    std::vector<std::string> grid;
    std::ranges::for_each(input_lines, [&grid](const auto& line)
    {
        grid.push_back(line);
    });

    const std::string word = "XMAS";

    int answer = count_word_occurrences(grid, word);

    std::cout << std::format("{}", answer);
}
