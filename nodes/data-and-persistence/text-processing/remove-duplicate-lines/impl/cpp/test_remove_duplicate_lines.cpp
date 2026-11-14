// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "remove_duplicate_lines.hpp"

namespace {

const std::filesystem::path input = "remove_duplicate_lines_input.txt";

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
    // 100000 lines with 1000 different lines.
    std::string many;
    std::vector<std::string> many_want;
    for (int i = 0; i < 100000; i++) {
        many += std::to_string(i % 1000) + "\n";
    }
    for (int i = 0; i < 1000; i++) {
        many_want.push_back(std::to_string(i));
    }

    const std::vector<test_case> cases = {
        {"simple", "b\na\nb\nc\na\n", {"b", "a", "c"}},
        {"adjacent", "a\na\nb\n", {"a", "b"}},
        {"no-duplicates", "c\nb\na\n", {"c", "b", "a"}},
        {"empty", "", {}},
        {"empty-lines", "\na\n\n\n", {"", "a"}},
        {"all-same", "x\nx\nx\n", {"x"}},
        {"no-final-newline", "a\nb\na", {"a", "b"}},
        {"crlf", "a\r\na\n", {"a\r", "a"}},
        {"case", "A\na\nA\n", {"A", "a"}},
        {"whitespace", "a \na\n a\na \n", {"a ", "a", " a"}},
        // U+00E9 is C3 A9. e and U+0301 is 65 CC 81.
        {"normalization", "\xc3\xa9\ne\xcc\x81\n\xc3\xa9\n", {"\xc3\xa9", "e\xcc\x81"}},
        {"many-lines", many, many_want},
    };
    int failed = 0;
    for (const auto& c : cases) {
        write(c.data);
        auto got = remove_duplicate_lines(input);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failed++;
        }
    }
    auto missing = remove_duplicate_lines("no/such/dir/missing.txt");
    if (missing || missing.error() != DedupError::cannot_open) {
        std::fprintf(stderr, "case not-found failed\n");
        failed++;
    }
    std::filesystem::remove(input);
    if (failed == 0) {
        std::puts("ok");
    }
    return failed == 0 ? 0 : 1;
}
