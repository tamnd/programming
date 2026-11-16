// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// Returns tmpl with $$ changed to $, and $name and ${name} changed to the
// value of name. It reads the template once, so a value is not read as a
// template. An unknown name throws std::out_of_range, and a $ without a name
// throws std::invalid_argument.
inline std::string substitute(std::string_view tmpl, const std::vector<std::string>& names,
                              const std::vector<std::string>& values) {
    std::unordered_map<std::string_view, std::string_view> table;
    for (std::size_t i = 0; i < names.size(); ++i) {
        table[names[i]] = values[i];
    }
    auto is_name_char = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; };
    std::string out;
    std::size_t i = 0;
    while (i < tmpl.size()) {
        const std::size_t dollar = tmpl.find('$', i);
        if (dollar == std::string_view::npos) {
            out += tmpl.substr(i);
            break;
        }
        out += tmpl.substr(i, dollar - i);
        std::size_t p = dollar + 1;
        if (p < tmpl.size() && tmpl[p] == '$') {
            out += '$';
            i = p + 1;
            continue;
        }
        const bool braced = p < tmpl.size() && tmpl[p] == '{';
        const std::size_t start = braced ? p + 1 : p;
        std::size_t end = start;
        if (end < tmpl.size() && (std::isalpha(static_cast<unsigned char>(tmpl[end])) != 0 || tmpl[end] == '_')) {
            while (end < tmpl.size() && is_name_char(tmpl[end])) {
                ++end;
            }
        }
        if (end == start || (braced && (end >= tmpl.size() || tmpl[end] != '}'))) {
            throw std::invalid_argument("a $ without a name");
        }
        out += table.at(tmpl.substr(start, end - start));
        i = braced ? end + 1 : end;
    }
    return out;
}

#endif
