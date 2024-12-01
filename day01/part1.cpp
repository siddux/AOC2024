#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <numeric>
#include <ranges>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    std::vector<ssize_t> left_list;
    std::vector<ssize_t> right_list;
    std::ranges::for_each(input_lines, [&left_list, &right_list](const auto& line)
    {
        auto split_view = line | std::views::split(' ') 
                               | std::views::filter([](auto&& subrange)
        { 
            return !subrange.empty(); 
        });

        auto it = split_view.begin();
        if (it != split_view.end()) {
            left_list.push_back(std::stoi(std::string((*it).begin(), (*it).end())));
            ++it;
        }
        if (it != split_view.end()) {
            right_list.push_back(std::stoi(std::string((*it).begin(), (*it).end())));
        }
    });

    std::ranges::sort(left_list, [](int a, int b) { return a > b; });
    std::ranges::sort(right_list, [](int a, int b) { return a > b; });

    auto answer = std::transform_reduce(
        right_list.begin(), right_list.begin() + right_list.size(),
        left_list.begin(),
        0,
        std::plus<>(),
        [](int right, int left) { return std::abs(right - left); }
    );

    std::cout << std::format("{}", answer);
}