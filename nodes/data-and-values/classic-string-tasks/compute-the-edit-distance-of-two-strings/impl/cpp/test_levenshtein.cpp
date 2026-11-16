// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

#include <cstddef>
#include <cstdio>

#include "levenshtein.hpp"

namespace {

struct test_case {
    const char* name;
    const char* a;
    const char* b;
    std::size_t want;
};

const test_case cases[] = {
    {"kitten-sitting", "kitten", "sitting", 3},
    {"rosettacode", "rosettacode", "raisethysword", 8},
    {"flaw-lawn", "flaw", "lawn", 2},
    {"both-empty", "", "", 0},
    {"empty-left", "", "abc", 3},
    {"empty-right", "abc", "", 3},
    {"equal", "same", "same", 0},
    {"one-substitution", "a", "b", 1},
    {"transposition", "ab", "ba", 2},
    {"case", "Apple", "apple", 1},
    {"disjoint", "abc", "xyz", 3},
    {"prefix", "abc", "abcdef", 3},
    {"intention", "intention", "execution", 5},
    {"space", "a b", "ab", 1},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (levenshtein(c.a, c.b) != c.want || levenshtein(c.b, c.a) != c.want) {
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
