// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

#include <cstdio>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "count_lines.hpp"

using namespace std::string_literals;

namespace {

const std::filesystem::path input = "count_lines_input.txt";

struct test_case {
    const char* name;
    std::string data;
    LineCount want;
};

}  // namespace

int main() {
    std::string many;
    for (int i = 0; i < 100000; ++i) {
        many += "x\n";
    }
    many += "end";  // more than one buffer of 64 KiB, and a last line with no LF

    // The suffix s makes a std::string that keeps a NUL byte inside the literal.
    const std::vector<test_case> cases = {
        {"lf", "alpha\nbeta\ngamma\n", {3, 3}},
        {"blank-lines", "first\n\n\nlast\n", {4, 4}},
        {"empty", "", {0, 0}},
        {"only-newline", "\n", {1, 1}},
        {"no-final-newline", "one\ntwo\nthree", {2, 3}},
        {"one-line-no-newline", "x", {0, 1}},
        {"crlf", "one\r\ntwo\r\n", {2, 2}},
        {"crlf-no-final-newline", "one\r\ntwo", {1, 2}},
        {"cr-only", "one\rtwo\r", {0, 1}},
        {"other-breaks", "a\vb\fc\xc2\x85" "d\xe2\x80\xa8" "e\n", {1, 1}},
        {"invalid-utf8", "ok\n\xff\xfe\n", {2, 2}},
        {"nul-bytes", "a\0b\n\0\n"s, {2, 2}},
        {"many-chunks", many, {100000, 100001}},
    };

    int failures = 0;
    for (const auto& c : cases) {
        {
            std::ofstream out(input, std::ios::binary);
            out << c.data;
        }
        const auto got = count_lines(input);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            ++failures;
        }
    }
    std::filesystem::remove(input);

    const auto missing = count_lines("count_lines_missing.txt");
    if (missing || missing.error() != CountError::cannot_open) {
        std::fprintf(stderr, "case missing failed\n");
        ++failures;
    }
    if (failures == 0) {
        std::puts("ok");
    }
    return failures == 0 ? 0 : 1;
}
