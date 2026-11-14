// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

#include "remove_duplicate_lines.hpp"

#include <fstream>
#include <unordered_set>

std::expected<std::vector<std::string>, DedupError> remove_duplicate_lines(const std::filesystem::path& path) {
    // Binary mode keeps each CR. In text mode, some systems change CRLF to LF.
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(DedupError::cannot_open);
    }
    std::unordered_set<std::string> seen;
    std::vector<std::string> lines;
    std::string line;
    // getline removes the LF. At the end of the file, it fails only if it
    // read no characters, so a last line with no LF is a line too.
    while (std::getline(in, line)) {
        // insert returns false as its second value if the set has the line.
        if (seen.insert(line).second) {
            lines.push_back(line);
        }
    }
    if (in.bad()) {
        return std::unexpected(DedupError::read_failed);
    }
    return lines;
}
