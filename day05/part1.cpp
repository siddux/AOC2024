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

bool validate_rules(const std::unordered_map<int, std::vector<int>>& rules, const std::vector<int>& pages) {
    for (auto [i, page] : pages | std::views::enumerate) {
        if (auto it = rules.find(page); it != rules.end()) {
            const auto& dependent_pages = it->second;

            for (int dependent_page : dependent_pages) {
                auto dependent_pos = std::ranges::find(pages, dependent_page);

                if (dependent_pos != pages.end() && std::distance(pages.begin(), dependent_pos) <= static_cast<int>(i)) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    auto it = std::ranges::find(input_lines, "");
    auto input_rules = input_lines | std::views::take(it - input_lines.begin());
    auto input_pages = input_lines | std::views::drop(it == input_lines.end() ? input_lines.size() : it - input_lines.begin() + 1);


    std::unordered_map<int, std::vector<int>> rules;
    std::ranges::for_each(input_rules, [&rules](const auto& line)
    {
        size_t delimiter_pos = line.find('|');
        int first_page = std::stoi(line.substr(0, delimiter_pos));
        int second_page = std::stoi(line.substr(delimiter_pos + 1));

        rules[first_page].push_back(second_page);
    });

    int answer = 0;
    std::ranges::for_each(input_pages, [&answer, &rules](const auto& line)
    {
        auto pages_range = line
        | std::views::split(',')
        | std::views::transform([](auto&& subrange) {
            std::string temp(subrange.begin(), subrange.end());
            return std::stoi(temp);
        });
        std::vector<int> pages(std::ranges::begin(pages_range), std::ranges::end(pages_range));

        if (validate_rules(rules, pages)) {
            answer += pages[pages.size() / 2];
        }
        
    });

    std::cout << std::format("{}", answer);
}
