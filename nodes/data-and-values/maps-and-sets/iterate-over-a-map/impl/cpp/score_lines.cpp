// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

#include "score_lines.hpp"

#include <map>

std::vector<std::string> score_lines(const std::unordered_map<std::string, int> &scores) {
    std::vector<std::string> lines;
    // An unordered_map gives no order. A std::map gives its entries in key order.
    const std::map<std::string, int> sorted(scores.begin(), scores.end());
    for (const auto &[key, value] : sorted) {
        lines.push_back(key + "=" + std::to_string(value));
    }
    int total = 0;
    for (const auto &[key, value] : scores) {
        total += value;
    }
    lines.push_back("total=" + std::to_string(total));
    return lines;
}
