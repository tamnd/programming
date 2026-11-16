// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

#include <cstddef>
#include <cstdio>

#include "pad.hpp"

namespace {

struct test_case {
    const char* name;
    const char* s;
    std::size_t width;
    char fill;
    const char* left;
    const char* right;
};

const test_case cases[] = {
    {"zeros", "7", 3, '0', "007", "700"},
    {"spaces", "abc", 6, ' ', "   abc", "abc   "},
    {"stars", "abc", 6, '*', "***abc", "abc***"},
    {"empty-string", "", 3, '-', "---", "---"},
    {"width-zero", "abc", 0, ' ', "abc", "abc"},
    {"exact-width", "abcd", 4, '.', "abcd", "abcd"},
    {"shorter-width", "hello", 3, ' ', "hello", "hello"},
    {"negative-number-text", "-42", 6, '0', "000-42", "-42000"},
    {"fill-same-as-text", "aa", 5, 'a', "aaaaa", "aaaaa"},
    {"one-char-wide", "x", 1, '_', "x", "x"},
    {"spaces-inside", "a b", 7, '.', "....a b", "a b...."},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const auto [left, right] = pad(c.s, c.width, c.fill);
        if (left != c.left || right != c.right) {
            std::printf("FAIL %s: got \"%s\" \"%s\"\n", c.name, left.c_str(), right.c_str());
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
