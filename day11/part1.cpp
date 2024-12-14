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

bool has_even_digits(ssize_t number) {
    std::string num_str = std::to_string(number);
    return num_str.length() % 2 == 0;
}

std::vector<ssize_t> split_stone(ssize_t number) {
    std::string num_str = std::to_string(number);

    if (num_str.length() % 2 != 0) {
        return {};
    }

    std::string first_half = num_str.substr(0, num_str.length() / 2);
    std::string second_half = num_str.substr(num_str.length() / 2);

    return {std::stoi(first_half), std::stoi(second_half)};
}

int main() {
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    const auto input_line = aoc_tools::readInput(input_path);

    std::vector<ssize_t> stones;
    std::istringstream stream(input_line);
    ssize_t number;

    while (stream >> number) {
        stones.push_back(number);
    }


    constexpr int blinks = 25;
    for (size_t i = 0; i < blinks; ++i) {
        std::vector<ssize_t> updated_stones;
        std::ranges::for_each(stones, [&](auto& stone)
        {
            if (stone == 0) {
                updated_stones.push_back(stone + 1);
            } else if (has_even_digits(stone)) {
                auto new_stones = split_stone(stone);
                updated_stones.insert(updated_stones.end(), new_stones.begin(), new_stones.end());
            } else {
                updated_stones.push_back(stone * 2024);
            }
            
        });
        stones = std::move(updated_stones);
    }

    std::cout << std::format("{}", stones.size());
}
