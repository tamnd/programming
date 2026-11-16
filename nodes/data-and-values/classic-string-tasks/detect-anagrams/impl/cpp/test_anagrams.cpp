// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

#include <cstdio>

#include "anagrams.hpp"

namespace {

struct test_case {
    const char* name;
    const char* a;
    const char* b;
    bool want;
};

const test_case cases[] = {
    {"listen-silent", "listen", "silent", true},
    {"dormitory", "Dormitory", "dirty room", true},
    {"hello-world", "hello", "world", false},
    {"both-empty", "", "", true},
    {"one-empty", "a", "", false},
    {"same-set-other-counts", "aab", "abb", false},
    {"extra-letter", "abc", "abcd", false},
    {"same-length-other-letters", "abc", "abd", false},
    {"case", "Listen", "SILENT", true},
    {"same-word", "stop", "stop", true},
    {"gentleman", "a gentleman", "elegant man", true},
    {"they-see", "The eyes!", "They see.", true},
    {"punctuation-only", "!?", "...", true},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (are_anagrams(c.a, c.b) != c.want || are_anagrams(c.b, c.a) != c.want) {
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
