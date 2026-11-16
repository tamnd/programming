// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

#include <cstdio>

#include "factorial.hpp"

namespace {

struct test_case {
    const char* name;
    int n;
    long long want;
};

const test_case cases[] = {
    {"five", 5, 120},
    {"ten", 10, 3628800},
    {"zero", 0, 1},
    {"one", 1, 1},
    {"largest-32-bit", 12, 479001600},
    {"beyond-32-bit", 13, 6227020800LL},
    {"largest-64-bit", 20, 2432902008176640000LL},
};

static_assert(factorial(5) == 120);

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const long long got = factorial(c.n);
        if (got != c.want) {
            std::printf("FAIL %s: %lld\n", c.name, got);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
