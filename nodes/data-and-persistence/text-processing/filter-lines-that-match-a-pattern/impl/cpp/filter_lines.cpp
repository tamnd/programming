// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

#include "filter_lines.hpp"

#include <fstream>
#include <regex>

std::expected<std::vector<std::string>, FilterError> filter_lines(const std::filesystem::path& path,
                                                                  const std::string& pattern) {
    // std::regex::extended selects the POSIX extended syntax, as grep -E does.
    // nosubs asks only if the line matches, not where.
    const std::regex re(pattern, std::regex::extended | std::regex::nosubs);
    // Binary mode keeps each CR. In text mode, some systems change CRLF to LF.
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(FilterError::cannot_open);
    }
    std::vector<std::string> lines;
    std::string line;
    // getline removes the LF. At the end of the file, it fails only if it
    // read no characters, so a last line with no LF is a line too.
    while (std::getline(in, line)) {
        // regex_search finds a match in some part of the line. regex_match
        // would need the whole line to match.
        if (std::regex_search(line, re)) {
            lines.push_back(line);
        }
    }
    if (in.bad()) {
        return std::unexpected(FilterError::read_failed);
    }
    return lines;
}
