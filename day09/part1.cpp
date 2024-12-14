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

int main() {
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    const auto input_line = aoc_tools::readInput(input_path);

    std::vector<int> memory;
    for (auto [index, ch] : std::views::enumerate(input_line)) {

        int num = ch - '0';

        if (index % 2 == 0) {
            for (size_t i = 0; i < num; ++i) {
                memory.push_back(static_cast<int>(index / 2));
            }
        } else {
            for (size_t i = 0; i < num; ++i) {
                memory.push_back(-1);
            }
        }
    }

    auto leftmost_dot = [&]() {
        return std::ranges::find(memory, -1);
    };

    auto rightmost_non_dot = [&]() {
        return std::ranges::find_if(memory.rbegin(), memory.rend(), [](int n) { return n != -1; }).base() - 1;
    };

    while (true) {
        auto left = leftmost_dot();
        auto right = rightmost_non_dot();

        if (left == memory.end() || left >= right) {
            break;
        }

        std::swap(*left, *right);
    }

    auto products = std::views::enumerate(memory)
              | std::views::take_while([](auto pair) {
                  auto [index, n] = pair;
                  return n >= 0; // Stop when a negative number is detected
              })
              | std::views::transform([](auto pair) {
                  auto [index, n] = pair;
                  return index * n;
              });

    ssize_t answer = std::ranges::fold_left(products, 0, std::plus{});

    std::cout << std::format("{}", answer);
}
