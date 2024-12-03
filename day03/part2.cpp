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
    bool enable_instruction = true;
    std::ranges::for_each(input_lines, [&answer, &enable_instruction](const auto& line)
    {
        std::regex regex(R"(mul\((\d{1,3}),(\d{1,3})\)|do(?:n't)?\(\))");


        auto begin = std::sregex_iterator(line.begin(), line.end(), regex);
        auto end = std::sregex_iterator();

        
        for (auto it = begin; it != end; ++it) {
            if (it->str() == "do()") {
                enable_instruction = true;
                continue;
            }

            if (it->str() == "don't()") {
                enable_instruction = false;
                continue;
            }
            
            if (enable_instruction) {
                answer += std::stoi((*it)[1]) * std::stoi((*it)[2]);
            }
        }
    });

    std::cout << std::format("{}", answer);
}