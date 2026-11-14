// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "filter_lines.hpp"

namespace {

const std::filesystem::path input = "filter_lines_input.txt";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
}

struct test_case {
    const char* name;
    std::string data;
    std::string pattern;
    std::vector<std::string> want;
};

}  // namespace

int main() {
    // 100000 lines with the numbers 0 to 99999. Five of them hold only nines.
    std::string numbers;
    for (int i = 0; i < 100000; i++) {
        numbers += std::to_string(i) + "\n";
    }

    const std::vector<test_case> cases = {
        {"literal", "apple\nbanana\ncherry\n", "an", {"banana"}},
        {"several", "cat\ndog\ncatalog\n", "cat", {"cat", "catalog"}},
        {"start", "ab\nba\n", "^a", {"ab"}},
        {"end", "ab\nba\n", "a$", {"ba"}},
        {"dot", "abc\nac\naxc\n", "a.c", {"abc", "axc"}},
        {"star", "ac\nabc\nabbc\nadc\n", "ab*c", {"ac", "abc", "abbc"}},
        {"whole-line", "a\nab\nba\n", "^a$", {"a"}},
        {"empty", "", "a", {}},
        {"no-match", "abc\n", "x", {}},
        {"empty-lines", "a\n\nb\n\n", "^$", {"", ""}},
        {"match-all", "a\n\nb\n", ".*", {"a", "", "b"}},
        {"duplicates", "a\nb\na\n", "a", {"a", "a"}},
        {"no-final-newline", "a\nab", "b", {"ab"}},
        {"crlf-end", "ab\r\nab\n", "b$", {"ab"}},
        {"crlf-dot", "a\r\n", "^a.$", {"a\r"}},
        {"case", "Apple\napple\n", "apple", {"apple"}},
        // U+00E9 is C3 A9.
        {"non-ascii", "caf\xc3\xa9\ncafe\n", "\xc3\xa9", {"caf\xc3\xa9"}},
        {"many-lines", numbers, "^9*$", {"9", "99", "999", "9999", "99999"}},
    };
    int failed = 0;
    for (const auto& c : cases) {
        write(c.data);
        auto got = filter_lines(input, c.pattern);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failed++;
        }
    }
    auto missing = filter_lines("no/such/dir/missing.txt", "a");
    if (missing || missing.error() != FilterError::cannot_open) {
        std::fprintf(stderr, "case not-found failed\n");
        failed++;
    }
    std::filesystem::remove(input);
    if (failed == 0) {
        std::puts("ok");
    }
    return failed == 0 ? 0 : 1;
}
