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
#include <vector>

#include "aoc_tools.hpp"

bool can_generate_result(const std::pair<ssize_t, std::vector<int>>& equation, size_t index = 0, ssize_t current_value = 0) {
    if (index == 0) {
        current_value = equation.second[index];
        index++;
    }

    if (index == equation.second.size()) {
        return current_value == equation.first;
    }

    // Add or multiply with the next number and recursively check
    ssize_t next_value = equation.second[index];
    return can_generate_result(equation, index + 1, current_value + next_value) ||
           can_generate_result(equation, index + 1, current_value * next_value);
}

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    ssize_t answer = 0;
    std::ranges::for_each(input_lines, [&answer](const auto& line)
    {
        std::pair<ssize_t, std::vector<int>> equation;
        std::istringstream stream(line);
        std::string result;
        std::string values;

        if (std::getline(stream, result, ':') && std::getline(stream, values)) {
            equation.first = std::stoll(result);

            std::istringstream values_stream(values);
            int num;
            while (values_stream >> num) {
                equation.second.push_back(num);
            }
        }

        if (can_generate_result(equation)) {
            answer += equation.first;
        }

    });

    std::cout << std::format("{}", answer);
}
