// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

#include <cstdio>
#include <vector>

#include "letters.hpp"

namespace {

struct test_case {
    const char* name;
    const char* text;
    std::vector<long long> want;
};

const test_case cases[] = {
    {"hello", "hello", {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"pangram", "The quick brown fox jumps over the lazy dog", {1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1}},
    {"mississippi", "Mississippi", {0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0}},
    {"empty", "", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"no-letters", "123 !?", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"mixed-case", "AaAa", {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"first-and-last", "aZ", {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
    {"only-z", "zzz", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}},
    {"white-space", " \t\n", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"sentence", "Count Each Letter, once.", {1, 0, 3, 0, 4, 0, 0, 1, 0, 0, 0, 1, 0, 2, 2, 0, 0, 1, 0, 3, 1, 0, 0, 0, 0, 0}},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (count_letters(c.text) != c.want) {
            std::printf("FAIL %s\n", c.name);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
