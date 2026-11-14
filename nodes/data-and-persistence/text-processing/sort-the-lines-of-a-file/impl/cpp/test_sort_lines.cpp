// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "sort_lines.hpp"

namespace {

const std::filesystem::path input = "sort_lines_input.txt";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
}

struct test_case {
    const char* name;
    std::string data;
    std::vector<std::string> want;
};

}  // namespace

int main() {
    std::string many;
    std::vector<std::string> many_want;
    for (int i = 0; i < 50000; i++) {
        many += "b\na\n";
    }
    many_want.insert(many_want.end(), 50000, "a");
    many_want.insert(many_want.end(), 50000, "b");

    const std::vector<test_case> cases = {
        {"simple", "pear\napple\nfig\n", {"apple", "fig", "pear"}},
        {"case", "b\nB\na\nA\n", {"A", "B", "a", "b"}},
        {"numbers", "10\n9\n100\n", {"10", "100", "9"}},
        {"empty", "", {}},
        {"one-empty-line", "\n", {""}},
        {"empty-lines", "b\n\na\n\n", {"", "", "a", "b"}},
        {"duplicates", "b\na\nb\n", {"a", "b", "b"}},
        {"no-final-newline", "b\na", {"a", "b"}},
        {"crlf", "b\r\na\r\n", {"a\r", "b\r"}},
        {"prefix", "ab\na\n", {"a", "ab"}},
        {"whitespace", "b\n a\n\tc\n", {"\tc", " a", "b"}},
        // The UTF-8 bytes of U+00E9 are C3 A9, which are above the byte of z.
        {"non-ascii", "\xc3\xa9\nz\ne\n", {"e", "z", "\xc3\xa9"}},
        // U+FF61 is EF BD A1 and U+1F600 is F0 9F 98 80.
        {"supplementary", "\xf0\x9f\x98\x80\n\xef\xbd\xa1\n", {"\xef\xbd\xa1", "\xf0\x9f\x98\x80"}},
        // U+2028 is E2 80 A8.
        {"other-line-breaks", "b\xe2\x80\xa8" "a\nb\x1c" "a\nb\x0c" "a\n", {"b\x0c" "a", "b\x1c" "a", "b\xe2\x80\xa8" "a"}},
        {"many-lines", many, many_want},
    };
    int failed = 0;
    for (const auto& c : cases) {
        write(c.data);
        auto got = sort_lines(input);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failed++;
        }
    }
    auto missing = sort_lines("no/such/dir/missing.txt");
    if (missing || missing.error() != SortError::cannot_open) {
        std::fprintf(stderr, "case not-found failed\n");
        failed++;
    }
    std::filesystem::remove(input);
    if (failed == 0) {
        std::puts("ok");
    }
    return failed == 0 ? 0 : 1;
}
