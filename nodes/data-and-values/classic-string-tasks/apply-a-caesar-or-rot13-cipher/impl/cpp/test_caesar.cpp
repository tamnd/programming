// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

#include <climits>
#include <cstdio>
#include <string>

#include "caesar.hpp"

namespace {

struct test_case {
    const char* name;
    const char* text;
    long long shift;
    const char* want;
};

const test_case cases[] = {
    {"hi-2", "HI", 2, "JK"},
    {"hi-20", "HI", 20, "BC"},
    {"rot13", "Hello, World!", 13, "Uryyb, Jbeyq!"},
    {"empty", "", 3, ""},
    {"zero", "abc", 0, "abc"},
    {"full-turn", "abc", 26, "abc"},
    {"minus-one", "abc", -1, "zab"},
    {"decode", "Uryyb, Jbeyq!", -13, "Hello, World!"},
    {"above-26", "xyz", 27, "yza"},
    {"wrap", "Zz", 1, "Aa"},
    {"non-letters", "123 !? [`{@", 5, "123 !? [`{@"},
    {"non-ascii", "café ü", 1, "dbgé ü"},
    {"large", "abc", 9007199254740991LL, "fgh"},
    {"large-negative", "abc", -9007199254740991LL, "vwx"},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const std::string got = caesar(c.text, c.shift);
        if (got != c.want || caesar(got, -c.shift) != c.text) {
            std::printf("FAIL %s: got \"%s\"\n", c.name, got.c_str());
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
