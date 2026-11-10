// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

#include "read_lines.hpp"

#include <ios>

std::vector<std::string> read_lines(std::istream& in) {
    std::vector<std::string> lines;
    std::string line;
    // getline removes the '\n'. It fails when no character is left.
    while (std::getline(in, line)) {
        if (line.ends_with('\r')) line.pop_back();
        lines.push_back(line);
    }
    // badbit means a read error; eofbit and failbit alone mean the end of input.
    if (in.bad()) throw std::ios_base::failure("read_lines: read error");
    return lines;
}
