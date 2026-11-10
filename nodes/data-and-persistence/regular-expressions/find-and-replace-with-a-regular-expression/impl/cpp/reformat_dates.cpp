// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

#include "reformat_dates.hpp"

#include <regex>
#include <string_view>

namespace {

// Returns s with a backslash before each special character of an ECMAScript pattern. The standard library has no
// function for this.
std::string escape(std::string_view s) {
    std::string out;
    for (char c : s) {
        if (std::string_view{R"(^$\.*+?()[]{}|)"}.contains(c)) {
            out += '\\';
        }
        out += c;
    }
    return out;
}

// The pattern of a date YYYY<sep>MM<sep>DD. The code checks the digits next to the date, because std::regex has no
// lookbehind.
std::regex date_pattern(const std::string& sep) {
    std::string s = escape(sep);
    return std::regex("([0-9]{4})" + s + "([0-9]{2})" + s + "([0-9]{2})");
}

bool is_digit_at(const std::string& text, std::size_t i) {
    return i < text.size() && text[i] >= '0' && text[i] <= '9';
}

// Calls f with each date in text that has no digit next to it.
template <typename F>
void each_date(const std::string& text, const std::regex& re, F f) {
    std::size_t pos = 0;
    std::smatch m;
    while (pos < text.size() && std::regex_search(text.begin() + static_cast<std::ptrdiff_t>(pos), text.end(), m, re)) {
        std::size_t start = pos + static_cast<std::size_t>(m.position(0));
        std::size_t end = start + static_cast<std::size_t>(m.length(0));
        if ((start > 0 && is_digit_at(text, start - 1)) || is_digit_at(text, end)) {
            pos = start + 1;  // a date can start inside this match
            continue;
        }
        f(start, end, m);
        pos = end;
    }
}

}  // namespace

std::string reformat_dates(const std::string& text, const std::string& from_sep, const std::string& to_sep) {
    std::string out;
    std::size_t last = 0;
    each_date(text, date_pattern(from_sep), [&](std::size_t start, std::size_t end, const std::smatch& m) {
        out.append(text, last, start - last);
        out += m.str(3) + to_sep + m.str(2) + to_sep + m.str(1);
        last = end;
    });
    out.append(text, last);
    return out;
}

std::vector<std::array<std::string, 3>> find_dates(const std::string& text, const std::string& sep) {
    std::vector<std::array<std::string, 3>> dates;
    each_date(text, date_pattern(sep), [&](std::size_t, std::size_t, const std::smatch& m) {
        dates.push_back({m.str(1), m.str(2), m.str(3)});
    });
    return dates;
}
