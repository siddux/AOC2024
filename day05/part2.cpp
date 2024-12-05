#include <algorithm>
#include <array>
#include <format>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <numeric>
#include <queue>
#include <source_location>
#include <string>
#include <ranges>
#include <unordered_set>
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

std::vector<int> topological_sort(
    const std::unordered_map<int, std::vector<int>>& rules,
    const std::vector<int>& pages
) {
    std::unordered_set<int> pages_set(pages.begin(), pages.end());

    std::unordered_map<int, int> in_degree;
    for (int p : pages) {
        in_degree[p] = 0;
    }

    std::unordered_map<int, std::vector<int>> graph;
    for (const auto& [page, deps] : rules) {
        if (pages_set.count(page)) {
            if (!graph.count(page)) {
                graph[page] = {};
            }

            for (int dep : deps) {
                if (pages_set.count(dep)) {
                    graph[page].push_back(dep);
                    in_degree[dep]++;
                }
            }
        }
    }

    // Kahn's Algorithm for Topological Sorting
    std::queue<int> q;
    for (auto& [node, deg] : in_degree) {
        if (deg == 0) {
            q.push(node);
        }
    }

    std::vector<int> sorted_order;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        sorted_order.push_back(current);

        for (int adj : graph[current]) {
            in_degree[adj]--;
            if (in_degree[adj] == 0) {
                q.push(adj);
            }
        }
    }

    if (sorted_order.size() == pages.size()) {
        return sorted_order;
    } else {
        return {};
    }
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

    std::vector<std::vector<int>> invalid_pages;
    std::ranges::for_each(input_pages, [&invalid_pages, &rules](const auto& line)
    {
        auto pages_range = line
        | std::views::split(',')
        | std::views::transform([](auto&& subrange) {
            std::string temp(subrange.begin(), subrange.end());
            return std::stoi(temp);
        });
        std::vector<int> pages(std::ranges::begin(pages_range), std::ranges::end(pages_range));

        if (!validate_rules(rules, pages)) {
            invalid_pages.push_back(pages);
        }
        
    });

    int answer = 0;
    std::ranges::for_each(invalid_pages, [&answer, &rules](const auto& line)
    {
        auto sorted_pages = topological_sort(rules, line);
        if (!sorted_pages.empty()) {
            answer += sorted_pages[sorted_pages.size() / 2];
        }
    });

    std::cout << std::format("{}", answer);
}
