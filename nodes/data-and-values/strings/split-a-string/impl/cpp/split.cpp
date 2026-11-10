// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

#include "split.hpp"

#include <regex>

std::vector<std::string_view> split(std::string_view s, std::string_view sep, std::size_t limit) {
    std::vector<std::string_view> fields;
    std::size_t start = 0;
    for (std::size_t at; (limit == 0 || fields.size() + 1 < limit) &&
                         (at = s.find(sep, start)) != std::string_view::npos;
         start = at + sep.size()) {
        fields.push_back(s.substr(start, at - start));
    }
    fields.push_back(s.substr(start));
    return fields;
}

std::vector<std::string_view> split_whitespace(std::string_view s) {
    constexpr std::string_view space = " \t\n";
    std::vector<std::string_view> fields;
    for (std::size_t start = s.find_first_not_of(space); start != std::string_view::npos;) {
        std::size_t end = s.find_first_of(space, start);
        fields.push_back(s.substr(start, end - start));
        start = s.find_first_not_of(space, end);
    }
    return fields;
}

std::vector<std::string> split_regex(const std::string& s, const std::string& pattern) {
    const std::regex re(pattern);
    std::vector<std::string> fields;
    auto start = s.cbegin();
    // std::sregex_token_iterator with -1 drops a trailing empty field, so the
    // loop calls std::regex_search itself.
    for (std::smatch m; std::regex_search(start, s.cend(), m, re) && m.length(0) > 0;
         start = m[0].second) {
        fields.emplace_back(start, m[0].first);
    }
    fields.emplace_back(start, s.cend());
    return fields;
}
