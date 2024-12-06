#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <numeric>
#include <source_location>
#include <string>
#include <ranges>
#include <utility>
#include <unordered_set>
#include <vector>

#include "aoc_tools.hpp"

std::pair<int, int> rotate(const std::pair<int, int> vec) {
    return {vec.second, -vec.first};
}

std::pair<int, int> move(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
    return {p1.first + p2.first, p1.second + p2.second};
}

struct position_set_hash {
    size_t operator()(const std::pair<int, int>& p) const {
        size_t h1 = std::hash<int>()(p.first);
        size_t h2 = std::hash<int>()(p.first);
        return h1 ^ (h2 << 1);
    }
};

struct state_set_hash {
    size_t operator()(const std::pair<std::pair<int, int>, std::pair<int, int>>& p) const {
        size_t h1 = std::hash<int>()(p.first.first);
        size_t h2 = std::hash<int>()(p.first.second);
        size_t h3 = std::hash<int>()(p.second.first);
        size_t h4 = std::hash<int>()(p.second.second);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

int main() {
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    const auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    int grid_rows = input_lines.size();
    int grid_cols = input_lines[0].size();
    std::vector<std::vector<char>> grid(grid_rows, std::vector<char>(grid_cols));
    ssize_t guard_col = 0;
    ssize_t guard_row = 0;

    for (const auto [row_index, row] : std::views::enumerate(input_lines)) {
        for (const auto [col_index, cell] : std::views::enumerate(row)) {
            if (cell == '^') {
                guard_col = col_index;
                guard_row = row_index;
            }
            grid[row_index][col_index] = cell;
        }
    }

    // We only care about obstacles placed on the original path.
    std::pair<int, int> guard_position{guard_row, guard_col};
    std::pair<int, int> direction{-1, 0};  // Initial direction: UP
    bool guard_in_map = true;
    std::unordered_set<std::pair<int, int>, position_set_hash> path_positions;
    while (guard_in_map) {
        auto next_position = move(guard_position, direction);
        if (next_position.first >= grid_rows || next_position.first < 0 ||
            next_position.second >= grid_cols || next_position.second < 0) {
            guard_in_map = false;
            continue;
        }

        if (grid[next_position.first][next_position.second] == '#') {
            direction = rotate(direction);
            continue;
        }

        guard_position = next_position;
        path_positions.insert(guard_position);
    }

    int answer = 0;

    for (const auto [row, col] : path_positions) {
        grid[row][col] = '#';

        std::pair<int, int> guard_position{guard_row, guard_col};
        std::pair<int, int> direction{-1, 0};  // Reset direction
        std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, state_set_hash> visited_states;

        bool guard_in_map = true;

        while (guard_in_map) {
            if (!visited_states.insert({guard_position, direction}).second) {
                ++answer;
                break;
            }

            auto next_position = move(guard_position, direction);
            if (next_position.first >= grid_rows || next_position.first < 0 ||
                next_position.second >= grid_cols || next_position.second < 0) {
                guard_in_map = false;
                continue;
            }

            if (grid[next_position.first][next_position.second] == '#') {
                direction = rotate(direction);
                continue;
            }

            guard_position = next_position;
        }
        grid[row][col] = 'O';
    }

    std::cout << std::format("{}", answer);
    return 0;
}
