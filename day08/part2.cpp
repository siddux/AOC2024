#include <algorithm>
#include <array>
#include <iostream>
#include <filesystem>
#include <format>
#include <unordered_map>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <source_location>
#include <string>
#include <utility>
#include <vector>

#include "aoc_tools.hpp"

using Coordinate = std::pair<int, int>;

std::vector<Coordinate> collect_coordinates(
    const std::vector<std::vector<char>>& matrix, char target_char) {
    std::vector<Coordinate> coords;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            if (matrix[i][j] == target_char) {
                coords.emplace_back(i, j);
            }
        }
    }
    return coords;
}

std::set<Coordinate> find_special_locations(
    const std::vector<std::vector<char>>& matrix, char target_char) {

    auto locations = collect_coordinates(matrix, target_char);
    std::set<Coordinate> result;

    for (size_t i = 0; i < locations.size(); ++i) {
        for (size_t j = i + 1; j < locations.size(); ++j) {
            Coordinate A = locations[i];
            Coordinate B = locations[j];
            
            auto is_collinear = [&](Coordinate P) {
                return (B.first - A.first) * (P.second - A.second) ==
                    (B.second - A.second) * (P.first - A.first);
            };

            auto rows = matrix.size();
            auto cols = matrix[0].size();
            for (const auto& [x, y] : std::views::cartesian_product(
                std::views::iota(std::size_t(0), rows), 
                std::views::iota(std::size_t(0), cols))) {
                    Coordinate P = {x, y};

                    // Check collinearity
                    if (!is_collinear(P)) continue;

                    result.insert(P);
            }
        }
    }

    return result;
}

std::set<char> find_unique_chars(const std::vector<std::vector<char>>& matrix) {
    std::set<char> unique_chars;
    for (const auto& row : matrix) {
        for (const auto& ch : row) {
            if (ch != '.') {
                unique_chars.insert(ch);
            }
        }
    }
    return unique_chars;
}

int main() {
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    const auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    auto grid_rows = input_lines.size();
    auto grid_cols = input_lines[0].size();
    std::vector<std::vector<char>> grid(grid_rows, std::vector<char>(grid_cols));
    for (const auto [row_index, row] : std::views::enumerate(input_lines)) {
        for (const auto [col_index, cell] : std::views::enumerate(row)) {
            grid[row_index][col_index] = cell;  
        }  
    }

    auto unique_chars = find_unique_chars(grid);

    std::map<char, std::set<Coordinate>> results;

    for (char ch : unique_chars) {
        results[ch] = find_special_locations(grid, ch);
    }

    std::set<Coordinate> combined_results;
    for (const auto& [ch, locations] : results) {
        combined_results.insert(locations.begin(), locations.end());
    }

    std::cout << std::format("{}", combined_results.size());
}
