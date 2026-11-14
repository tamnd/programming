// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

#include "first_and_last_lines.hpp"

#include <deque>
#include <fstream>

std::expected<FirstAndLast, LinesError> first_and_last_lines(const std::filesystem::path& path,
                                                             std::size_t count) {
    // Binary mode keeps each CR. In text mode, some systems change CRLF to LF.
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(LinesError::cannot_open);
    }
    std::vector<std::string> first;
    // last holds the last count lines that the loop has seen. When it is full,
    // pop_front removes the oldest line before push_back adds the new one.
    std::deque<std::string> last;
    std::string line;
    // getline removes the LF. At the end of the file, it fails only if it
    // read no characters, so a last line with no LF is a line too.
    while (count > 0 && std::getline(in, line)) {
        if (first.size() < count) {
            first.push_back(line);
        }
        if (last.size() == count) {
            last.pop_front();
        }
        last.push_back(std::move(line));
    }
    if (in.bad()) {
        return std::unexpected(LinesError::read_failed);
    }
    return FirstAndLast{std::move(first), {last.begin(), last.end()}};
}
