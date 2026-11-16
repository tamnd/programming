// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

#ifndef WRAP_HPP
#define WRAP_HPP

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// Returns the lines of text, each at most width characters long, with the
// greedy method. A word longer than width is a line of its own.
inline std::vector<std::string> wrap(const std::string& text, std::size_t width) {
    std::vector<std::string> lines;
    std::istringstream in(text);
    std::string line;
    std::string word;
    while (in >> word) {
        if (line.empty()) {
            line = word;
        } else if (line.size() + 1 + word.size() <= width) {
            line += ' ';
            line += word;
        } else {
            lines.push_back(line);
            line = word;
        }
    }
    if (!line.empty()) {
        lines.push_back(line);
    }
    return lines;
}

#endif
