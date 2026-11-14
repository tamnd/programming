// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

#include <cstdio>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "read_line.hpp"

namespace {

const std::filesystem::path input = "read_line_input.txt";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
}

struct test_case {
    const char* name;
    std::string data;
    int number;
    std::optional<std::string> want;
};

}  // namespace

int main() {
    // 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
    std::string numbers;
    for (int i = 0; i < 100000; i++) {
        numbers += std::to_string(i) + "\n";
    }

    const std::vector<test_case> cases = {
        {"first", "a\nb\nc\n", 1, "a"},
        {"middle", "a\nb\nc\n", 2, "b"},
        {"last", "a\nb\nc\n", 3, "c"},
        {"past-the-end", "a\nb\n", 3, std::nullopt},
        {"zero", "a\n", 0, std::nullopt},
        {"negative", "a\n", -1, std::nullopt},
        {"empty", "", 1, std::nullopt},
        {"empty-line", "a\n\nb\n", 2, ""},
        {"only-newline", "\n", 1, ""},
        {"no-final-newline", "a\nb", 2, "b"},
        {"crlf", "a\r\nb\r\n", 1, "a\r"},
        {"lone-cr", "a\rb\nc\n", 2, "c"},
        // U+00E9 is C3 A9, and U+00EB is C3 AB.
        {"non-ascii", "caf\xc3\xa9\nzo\xc3\xab\n", 2, "zo\xc3\xab"},
        {"many-lines", numbers, 100000, "99999"},
        {"many-lines-past-the-end", numbers, 100001, std::nullopt},
    };
    int failed = 0;
    for (const auto& c : cases) {
        write(c.data);
        auto got = read_line(input, c.number);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failed++;
        }
    }
    auto missing = read_line("no/such/dir/missing.txt", 1);
    if (missing || missing.error() != LineError::cannot_open) {
        std::fprintf(stderr, "case not-found failed\n");
        failed++;
    }
    std::filesystem::remove(input);
    if (failed == 0) {
        std::puts("ok");
    }
    return failed == 0 ? 0 : 1;
}
