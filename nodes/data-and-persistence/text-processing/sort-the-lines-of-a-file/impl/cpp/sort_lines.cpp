// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

#include "sort_lines.hpp"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>

std::expected<std::vector<std::string>, SortError> sort_lines(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(SortError::cannot_open);
    }
    const std::string data{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    if (in.bad()) {
        return std::unexpected(SortError::read_failed);
    }
    std::vector<std::string> lines;
    std::size_t start = 0;
    while (start < data.size()) {
        std::size_t end = data.find('\n', start);
        if (end == std::string::npos) {
            end = data.size();
        }
        lines.emplace_back(data, start, end - start);
        start = end + 1;
    }
    // std::string compares its chars with std::char_traits<char>, which
    // compares them as unsigned char, so the order is the order of the bytes.
    std::ranges::sort(lines);
    return lines;
}
