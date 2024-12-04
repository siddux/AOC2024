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

std::vector<std::vector<std::string>> generate_all_orientations(const std::vector<std::string>& pattern) {
    // Helper functions to rotate and reflect the pattern
    auto rotate = [](const std::vector<std::string>& p) {
        size_t N = p.size();
        std::vector<std::string> result(N, std::string(N, ' '));
        std::ranges::for_each(std::views::iota(size_t{0}, N * N), [&](size_t k) {
            size_t i = k / N;
            size_t j = k % N;
            result[j][N - i - 1] = p[i][j];
        });
        return result;
    };

    auto reflect = [](const std::vector<std::string>& p) {
        size_t N = p.size();
        std::vector<std::string> result = p;
        std::ranges::for_each(std::views::iota(size_t{0}, N), [&](size_t i) {
            std::ranges::reverse(result[i]);
        });
        return result;
    };

    // As pattern has some sort of simetry we only need 4 variations
    std::vector<std::vector<std::string>> orientations;
    orientations.push_back(pattern);
    orientations.push_back(rotate(pattern));
    orientations.push_back(reflect(pattern));
    orientations.push_back(rotate(reflect(pattern)));

    return orientations;
}

int count_pattern_occurrences(const std::vector<std::string>& grid, const std::vector<std::vector<std::string>>& patterns) {
    const int rows = grid.size();
    const int cols = grid[0].size();
    const int pattern_rows = patterns[0].size();
    const int pattern_cols = patterns[0][0].size();


    auto is_valid = [&](int x, int y) {
        return x >= 0 && y >= 0 && x + pattern_rows <= rows && y + pattern_cols <= cols;
    };

    int total_positions = (rows - pattern_rows + 1) * (cols - pattern_cols + 1);

    auto positions = std::views::iota(0, total_positions);

    int count = std::accumulate(
        positions.begin(), positions.end(), 0,
        [&](int total, int idx) {
            int x = idx / (cols - pattern_cols + 1);
            int y = idx % (cols - pattern_cols + 1);

            if (!is_valid(x, y)) {
                return total;
            }

            // Check if any orientation matches starting from (x, y)
            bool any_match = std::ranges::any_of(
                patterns,
                [&](const std::vector<std::string>& pattern) {
                    bool matches = std::ranges::all_of(
                        std::views::iota(0, pattern_rows * pattern_cols),
                        [&](int k) {
                            int i = k / pattern_cols;
                            int j = k % pattern_cols;
                            char p_char = pattern[i][j];
                            if (p_char == '.') {
                                return true; // Wildcard matches any character
                            }
                            char g_char = grid[x + i][y + j];
                            return g_char == p_char;
                        }
                    );
                    return matches;
                }
            );

            return total + (any_match ? 1 : 0);
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

    std::vector<std::string> pattern = {
        "M.S",
        ".A.",
        "M.S"
    };

    int answer = count_pattern_occurrences(grid, generate_all_orientations(pattern));

    std::cout << std::format("{}", answer);
}
