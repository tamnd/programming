// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

#include <cstdio>

#include "prime.hpp"

namespace {

struct test_case {
    const char* name;
    long long n;
    bool want;
};

const test_case cases[] = {
    {"small-prime", 17, true},
    {"small-composite", 15, false},
    {"even", 100, false},
    {"odd-composite", 91, false},
    {"negative", -7, false},
    {"zero", 0, false},
    {"one", 1, false},
    {"two", 2, true},
    {"three", 3, true},
    {"square-of-prime", 25, false},
    {"large-square", 4293001441LL, false},
    {"carmichael", 561, false},
    {"large-prime", 1000000007, true},
    {"mersenne-prime", 2147483647, true},
    {"large-composite", 2147483649LL, false},
};

static_assert(is_prime(97) && !is_prime(91));

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (is_prime(c.n) != c.want) {
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
