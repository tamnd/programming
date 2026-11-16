// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

#include <cstdio>

#include "palindrome.hpp"

namespace {

struct test_case {
    const char* name;
    const char* text;
    bool want;
};

const test_case cases[] = {
    {"racecar", "racecar", true},
    {"panama", "A man, a plan, a canal: Panama", true},
    {"hello", "hello", false},
    {"empty", "", true},
    {"only-punctuation", "?! ...", true},
    {"one-letter", "a", true},
    {"two-letters", "ab", false},
    {"mixed-case", "Aa", true},
    {"nixon", "No 'x' in Nixon", true},
    {"cat-i-saw", "Was it a car or a cat I saw?", true},
    {"digits", "12321", true},
    {"digits-not", "123", false},
    {"zero-p", "0P", false},
    {"race-a-car", "race a car", false},
    {"even-near", "abca", false},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (is_palindrome(c.text) != c.want) {
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
