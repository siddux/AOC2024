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

std::optional<size_t> find_left_gap(const std::vector<int>& memory, size_t file_start, size_t file_length) {
    size_t gap_start = 0;
    while (gap_start < file_start) {
        gap_start = std::find_if(memory.begin() + gap_start, memory.begin() + file_start, 
                                 [](int n){ return n == -1; }) - memory.begin();
        if (gap_start >= file_start) break;

        size_t gap_end = std::find_if(memory.begin() + gap_start, memory.begin() + file_start,
                                      [](int n){ return n != -1; }) - memory.begin();
        size_t gap_size = gap_end - gap_start;
        if (gap_size >= file_length) {
            return gap_start;
        }
        gap_start = gap_end;
    }
    return std::nullopt;
}


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

    struct FileInfo {
        int id;
        size_t start;
        size_t end; // end is exclusive
        size_t length() const { return end - start; }
    };

    std::vector<FileInfo> files;
    {
        size_t i = 0;
        while (i < memory.size()) {
            if (memory[i] == -1) {
                i++;
                continue;
            }
            int id = memory[i];
            size_t start = i;
            while (i < memory.size() && memory[i] == id) {
                i++;
            }
            files.push_back({id, start, i});
        }
    }

    std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
        return a.id > b.id;
    });

    for (auto& file : files) {
        size_t length = file.length();
        auto gap_pos = find_left_gap(memory, file.start, length);
        if (gap_pos) {
            std::move(memory.begin() + file.start, memory.begin() + file.end, memory.begin() + *gap_pos);
            std::fill(memory.begin() + file.start, memory.begin() + file.end, -1);

            file.end = *gap_pos + length;
            file.start = *gap_pos;
        }
    }

    auto products = memory
        | std::views::enumerate
        | std::views::transform([](auto pair) {
              auto [index, n] = pair;
              return n < 0 ? 0 : index * n; // Compute product, insert 0 for negatives
          });

    ssize_t answer = std::ranges::fold_left(products, 0, std::plus{});

    std::cout << std::format("{}", answer);
}
