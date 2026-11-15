// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

#include "diff_two_texts.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>

std::vector<std::string_view> split_lines(std::string_view text) {
    std::vector<std::string_view> lines;
    while (!text.empty()) {
        std::size_t lf = text.find('\n');
        lines.push_back(text.substr(0, lf));
        text.remove_prefix(lf == std::string_view::npos ? text.size() : lf + 1);
    }
    return lines;
}

std::string diff_lines(std::string_view old_text, std::string_view new_text) {
    const auto a = split_lines(old_text);
    const auto b = split_lines(new_text);
    const std::size_t n = a.size(), m = b.size(), w = m + 1;
    // lcs[i * w + j] is the length of a longest common subsequence of the
    // lines from a[i] and from b[j]. Row n and column m stay 0.
    std::vector<std::size_t> lcs((n + 1) * w);
    for (std::size_t i = n; i-- > 0;) {
        for (std::size_t j = m; j-- > 0;) {
            lcs[i * w + j] =
                a[i] == b[j] ? lcs[(i + 1) * w + j + 1] + 1 : std::max(lcs[(i + 1) * w + j], lcs[i * w + j + 1]);
        }
    }
    std::string out;
    auto line = [&out](char mark, std::string_view s) {
        out += mark;
        out += s;
        out += '\n';
    };
    std::size_t i = 0, j = 0;
    while (i < n && j < m) {
        if (a[i] == b[j]) {
            line(' ', a[i]);
            i++;
            j++;
        } else if (lcs[(i + 1) * w + j] >= lcs[i * w + j + 1]) {
            line('-', a[i++]);
        } else {
            line('+', b[j++]);
        }
    }
    for (; i < n; i++) {
        line('-', a[i]);
    }
    for (; j < m; j++) {
        line('+', b[j]);
    }
    return out;
}

// Reads the whole file in binary mode, so that each CR stays.
static std::expected<std::string, DiffError> read_file(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(DiffError::cannot_open);
    }
    std::string data{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    if (in.bad()) {
        return std::unexpected(DiffError::read_failed);
    }
    return data;
}

std::expected<std::string, DiffError> diff_files(const std::filesystem::path& old_path,
                                                 const std::filesystem::path& new_path) {
    auto old_text = read_file(old_path);
    if (!old_text) {
        return std::unexpected(old_text.error());
    }
    auto new_text = read_file(new_path);
    if (!new_text) {
        return std::unexpected(new_text.error());
    }
    return diff_lines(*old_text, *new_text);
}
