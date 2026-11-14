// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

#include "align_columns.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace {

// Returns the number of code points in the UTF-8 text. A byte from 0x80 to
// 0xBF continues a code point, so the code counts the other bytes.
std::size_t code_points(std::string_view s) {
    return static_cast<std::size_t>(
        std::ranges::count_if(s, [](char c) { return (static_cast<unsigned char>(c) & 0xC0) != 0x80; }));
}

// Splits s at each sep. A sep at the end does not start a new part, and an
// empty s has no parts.
std::vector<std::string_view> split(std::string_view s, char sep) {
    std::vector<std::string_view> parts;
    while (!s.empty()) {
        std::size_t at = s.find(sep);
        parts.push_back(s.substr(0, at));
        s = at == std::string_view::npos ? std::string_view{} : s.substr(at + 1);
    }
    return parts;
}

}  // namespace

std::string align_columns(std::string_view text, std::string_view align) {
    if (align != "left" && align != "right" && align != "center") {
        throw std::invalid_argument("align must be left, right or center");
    }
    std::vector<std::vector<std::string_view>> rows;
    std::vector<std::size_t> widths;
    for (std::string_view line : split(text, '\n')) {
        rows.push_back(split(line, '$'));
        const auto& fields = rows.back();
        if (fields.size() > widths.size()) {
            widths.resize(fields.size());
        }
        for (std::size_t j = 0; j < fields.size(); j++) {
            widths[j] = std::max(widths[j], code_points(fields[j]));
        }
    }
    std::string out;
    for (const auto& fields : rows) {
        for (std::size_t j = 0; j < fields.size(); j++) {
            std::size_t pad = widths[j] - code_points(fields[j]);
            std::size_t left = align == "left" ? 0 : align == "right" ? pad : pad / 2;
            if (j > 0) {
                out += ' ';
            }
            out.append(left, ' ');
            out += fields[j];
            if (j + 1 < fields.size()) {
                out.append(pad - left, ' ');
            }
        }
        out += '\n';
    }
    return out;
}
