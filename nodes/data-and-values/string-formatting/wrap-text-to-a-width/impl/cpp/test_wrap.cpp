// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

#include "wrap.hpp"

namespace {

struct test_case {
    const char* name;
    const char* text;
    std::size_t width;
    std::vector<std::string> want;
};

const test_case cases[] = {
    {"fox-ten", "The quick brown fox jumps over the lazy dog", 10, {"The quick", "brown fox", "jumps over", "the lazy", "dog"}},
    {"fox-twenty", "The quick brown fox jumps over the lazy dog", 20, {"The quick brown fox", "jumps over the lazy", "dog"}},
    {"sentence", "Wrap text to a width with a greedy loop.", 15, {"Wrap text to a", "width with a", "greedy loop."}},
    {"empty", "", 10, {}},
    {"only-spaces", "   ", 5, {}},
    {"fits", "short text", 20, {"short text"}},
    {"exact-fit", "abcde fghij", 5, {"abcde", "fghij"}},
    {"long-word", "a supercalifragilistic word", 10, {"a", "supercalifragilistic", "word"}},
    {"many-spaces", "one   two\tthree\nfour", 9, {"one two", "three", "four"}},
    {"width-one", "a b c", 1, {"a", "b", "c"}},
    {"line-exact", "ab cd ef", 5, {"ab cd", "ef"}},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (wrap(c.text, c.width) != c.want) {
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
