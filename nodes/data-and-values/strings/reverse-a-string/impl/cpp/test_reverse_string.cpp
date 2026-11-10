// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

#include <cstdio>
#include <string>
#include <utility>
#include <vector>

#include "reverse_string.hpp"

int main() {
    int failures = 0;
    const std::vector<std::pair<std::string, std::pair<std::string, std::string>>> cases = {
        {"ascii", {"hello", "olleh"}},
        {"two-words", {"abc def", "fed cba"}},
        {"palindrome", {"racecar", "racecar"}},
        {"empty", {"", ""}},
        {"single", {"a", "a"}},
        {"two-byte", {"naïve", "evïan"}},
        {"three-byte", {"日本語", "語本日"}},
        {"astral", {"a🙂b", "b🙂a"}},
        {"two-astral", {"🙂😀", "😀🙂"}},
        {"combining-mark", {"e\u0301x", "x\u0301e"}},
        {"flag", {"🇫🇷", "🇷🇫"}},
    };
    for (const auto& [name, io] : cases) {
        auto got = reverse_code_points(io.first);
        if (!got.has_value() || *got != io.second) {
            std::fprintf(stderr, "case %s failed\n", name.c_str());
            ++failures;
        }
    }

    const std::vector<std::pair<std::string, std::string>> invalid = {
        {"invalid-byte", "\xff\xfe"},
        {"truncated", "caf\xc3"},
        {"encoded-surrogate", "a\xed\xa0\x80" "b"},
        {"overlong", "\xc0\xaf"},
    };
    for (const auto& [name, s] : invalid) {
        auto got = reverse_code_points(s);
        if (got.has_value() || got.error() != ReverseError::invalid_utf8) {
            std::fprintf(stderr, "case %s: no invalid_utf8 error\n", name.c_str());
            ++failures;
        }
    }

    if (failures != 0) return 1;
    std::puts("all tests passed");
    return 0;
}
