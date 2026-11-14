// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "first_and_last_lines.hpp"

namespace {

const std::filesystem::path input = "first_and_last_input.txt";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
}

// The numbers from and up to to, but not to.
std::vector<std::string> numbers(int from, int to) {
    std::vector<std::string> v;
    for (int i = from; i < to; i++) {
        v.push_back(std::to_string(i));
    }
    return v;
}

std::string text(const std::vector<std::string>& lines) {
    std::string s;
    for (const auto& l : lines) {
        s += l + "\n";
    }
    return s;
}

struct test_case {
    const char* name;
    std::string data;
    std::size_t count;
    std::vector<std::string> first;
    std::vector<std::string> last;
};

}  // namespace

int main() {
    const std::vector<test_case> cases = {
        {"two", "a\nb\nc\nd\ne\n", 2, {"a", "b"}, {"d", "e"}},
        {"one", "first\nmiddle\nlast\n", 1, {"first"}, {"last"}},
        {"ten", text(numbers(1, 13)), 10, numbers(1, 11), numbers(3, 13)},
        {"empty", "", 2, {}, {}},
        {"zero", "a\nb\n", 0, {}, {}},
        {"short", "a\nb\n", 5, {"a", "b"}, {"a", "b"}},
        {"exact", "a\nb\n", 2, {"a", "b"}, {"a", "b"}},
        {"overlap", "a\nb\nc\n", 2, {"a", "b"}, {"b", "c"}},
        {"one-line", "a\n", 1, {"a"}, {"a"}},
        {"empty-lines", "\n\na\n\n", 1, {""}, {""}},
        {"only-newline", "\n", 1, {""}, {""}},
        {"no-final-newline", "a\nb\nc", 1, {"a"}, {"c"}},
        {"crlf", "a\r\nb\r\n", 1, {"a\r"}, {"b\r"}},
        // U+00E9 is C3 A9, U+00EF is C3 AF, and U+00EB is C3 AB.
        {"non-ascii", "caf\xc3\xa9\nna\xc3\xafve\nzo\xc3\xab\n", 1, {"caf\xc3\xa9"}, {"zo\xc3\xab"}},
        // 100000 lines with the numbers 0 to 99999.
        {"many-lines", text(numbers(0, 100000)), 3, numbers(0, 3), numbers(99997, 100000)},
    };
    int failed = 0;
    for (const auto& c : cases) {
        write(c.data);
        auto got = first_and_last_lines(input, c.count);
        if (!got || got->first != c.first || got->second != c.last) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failed++;
        }
    }
    auto missing = first_and_last_lines("no/such/dir/missing.txt", 1);
    if (missing || missing.error() != LinesError::cannot_open) {
        std::fprintf(stderr, "case not-found failed\n");
        failed++;
    }
    std::filesystem::remove(input);
    if (failed == 0) {
        std::puts("ok");
    }
    return failed == 0 ? 0 : 1;
}
