#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <numeric>
#include <ranges>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

bool is_increasing_within_limits(const std::vector<int>& numbers, const int min_diff, const int max_diff)
{
    return std::ranges::adjacent_find(numbers, [&](int a, int b) {
            int diff = b - a;
            return diff < min_diff || diff > max_diff;
        }) == numbers.end();
}

bool is_decreasing_within_limits(const std::vector<int>& numbers, const int min_diff, const int max_diff)
{
    return std::ranges::adjacent_find(numbers, [&](int a, int b) {
            int diff = a - b;
            return diff < min_diff || diff > max_diff;
        }) == numbers.end();
}

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    int answer = 0;
    std::ranges::for_each(input_lines, [&answer](const auto& line)
    {
        std::vector<int> report;
        for (auto&& char_level : std::ranges::views::split(line, ' ')) {
            std::string subrange(char_level.begin(), char_level.end());
            int level = 0;
            auto [ptr, ec] = std::from_chars(subrange.data(), subrange.data() + subrange.size(), level);
            if (ec == std::errc{}) {
                report.push_back(level);
            }
        }

        constexpr int min_diff = 1;
        constexpr int max_diff = 3;
        auto is_valid = is_increasing_within_limits(report, min_diff, max_diff) ||
                        is_decreasing_within_limits(report, min_diff, max_diff);

        
        if (is_valid) {++answer; return;}

        for (size_t i = 0; i < report.size(); ++i) {
            
            std::vector<int> modified_report;
            modified_report.insert(modified_report.end(), report.begin(), report.begin() + i);
            modified_report.insert(modified_report.end(), report.begin() + i + 1, report.end());

            if (is_increasing_within_limits(modified_report, min_diff, max_diff) ||
                is_decreasing_within_limits(modified_report, min_diff, max_diff)) {
                    ++answer;
                    return;
            }
        }
        
    });

    std::cout << std::format("{}", answer);
}