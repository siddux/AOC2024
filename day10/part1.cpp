#include <algorithm>
#include <array>
#include <iostream>
#include <filesystem>
#include <format>
#include <functional>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <source_location>
#include <string>
#include <utility>
#include <vector>

#include "aoc_tools.hpp"

const std::vector<std::pair<int, int>> directions = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};

bool is_in_bounds(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

std::vector<std::vector<std::pair<int, int>>> find_paths(const std::map<int, std::vector<std::pair<int, int>>>& heights, int rows, int cols) {
    const auto& start_positions = heights.at(0);

    std::vector<std::vector<std::pair<int, int>>> all_paths;
    std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> unique_start_end_pairs;

    std::function<void(std::pair<int, int>, int, std::vector<std::pair<int, int>>&)>
    dfs = [&](std::pair<int, int> current, int current_height, std::vector<std::pair<int, int>>& path) {
        path.push_back(current);

        if (current_height == 9) {
            auto start = path.front();
            auto end = path.back();

            if (unique_start_end_pairs.emplace(start, end).second) {
                all_paths.push_back(path); // Add path if unique
            }
            path.pop_back();
            return;
        }

        auto next_height = current_height + 1;
        if (heights.count(next_height)) {
            const auto& next_positions = heights.at(next_height);

            for (const auto& next : next_positions) {
                if ((std::abs(current.first - next.first) + std::abs(current.second - next.second) == 1) && 
                     std::find(path.begin(), path.end(), next) == path.end()) {
                    dfs(next, next_height, path);
                }
            }
        }

        path.pop_back();
    };

    for (const auto& start : start_positions) {
        std::vector<std::pair<int, int>> path;
        dfs(start, 0, path);
    }
    
    return all_paths;
}

int main() {
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    const auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    int rows = input_lines.size();
    int cols = input_lines[0].size();
    std::map<int, std::vector<std::pair<int, int>>> heights;
    for (const auto [row_index, row] : std::views::enumerate(input_lines)) {
        for (const auto [col_index, cell] : std::views::enumerate(row)) {
            if (cell == '.') continue;
            auto height = cell - '0';
            heights[height].emplace_back(row_index, col_index);
        }  
    }

    auto paths = find_paths(heights, rows, cols);

    std::cout << std::format("{}", paths.size());
}
