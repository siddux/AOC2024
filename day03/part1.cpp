#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <numeric>
#include <regex>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    int answer = 0;
    std::ranges::for_each(input_lines, [&answer](const auto& line)
    {
        std::regex regex(R"(mul\((\d{1,3}),(\d{1,3})\))");

        auto begin = std::sregex_iterator(line.begin(), line.end(), regex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {         
            answer += std::stoi((*it)[1]) * std::stoi((*it)[2]);
        }
    });

    std::cout << std::format("{}", answer);
}