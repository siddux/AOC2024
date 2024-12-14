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

std::pair<ssize_t, ssize_t> split_stone(ssize_t number) {
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

    std::map<ssize_t, ssize_t> stones;
    std::istringstream stream(input_line);
    ssize_t number;

    while (stream >> number) {
        stones[number]++;
    }


    constexpr int blinks = 75;
    for (size_t i = 0; i < blinks; ++i) {
        std::map<ssize_t, ssize_t> updated_stones;
        for (const auto& [stone, count] : stones) {
            if (stone == 0) {
                updated_stones[1] += count;
            } else if (has_even_digits(stone)) {
                auto [left, right] = split_stone(stone);
                updated_stones[left] += count;
                updated_stones[right] += count;
            } else {
                
                updated_stones[stone * 2024] += count;
            }
            
        }
        stones = std::move(updated_stones);
    }

    ssize_t answer = 0;
    for (const auto& [_, count] : stones) answer += count;

    std::cout << std::format("{}", answer);
}
