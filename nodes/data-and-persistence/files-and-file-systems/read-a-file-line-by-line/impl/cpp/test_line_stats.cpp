// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "line_stats.hpp"

namespace {

const std::filesystem::path input = "line_stats_input.txt";

void write_file(std::string_view data) {
    std::ofstream out(input, std::ios::binary);
    out << data;
}

int failures = 0;

void check(std::string_view name, bool ok) {
    if (!ok) {
        std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

}  // namespace

int main() {
    using namespace std::string_literals;
    struct Case {
        std::string name, data;
        LineStats want;
    };
    const std::vector<Case> cases = {
        {"lf", "alpha\nbeta\ngamma\n", {3, 5}},
        {"blank-lines", "first\n\n\nlast line\n", {4, 9}},
        {"empty", "", {0, 0}},
        {"only-newline", "\n", {1, 0}},
        {"no-final-newline", "one\ntwo\nthree", {3, 5}},
        {"crlf", "one\r\ntwo\r\nthree\r\n", {3, 5}},
        {"crlf-no-final-newline", "one\r\ntwo\r\nthree", {3, 5}},
        {"mixed-endings", "one\r\ntwo\nthree\r\n", {3, 5}},
        {"whitespace", "  two spaces\ttab \n", {1, 17}},
        {"backslashes", "C:\\temp\\new\n", {1, 11}},
        {"long-line", std::string(1 << 20, 'x') + "\nend\n", {2, 1 << 20}},
        {"non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", {3, 10}},
    };
    for (const auto& c : cases) {
        write_file(c.data);
        auto got = line_stats(input);
        check(c.name, got.has_value() && *got == c.want);
    }

    const std::vector<std::pair<std::string, std::string>> invalid = {
        {"invalid-byte", "ok\n\xff\xfe\n"s},
        {"truncated-at-end", "ok\ncaf\xc3"s},
        {"encoded-surrogate", "a\xed\xa0\x80"s + "b\n"},
        {"overlong", "\xc0\xaf\n"s},
    };
    for (const auto& [name, data] : invalid) {
        write_file(data);
        auto got = line_stats(input);
        check(name, !got && got.error() == ReadError::invalid_utf8);
    }

    auto missing = line_stats("missing.txt");
    check("missing", !missing && missing.error() == ReadError::cannot_open);

    std::filesystem::remove(input);
    if (failures > 0) return EXIT_FAILURE;
    std::puts("all tests passed");
}
