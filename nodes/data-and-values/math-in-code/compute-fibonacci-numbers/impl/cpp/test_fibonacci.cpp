// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

#include <cstdio>

#include "fibonacci.hpp"

namespace {

struct test_case {
    const char* name;
    int n;
    long long want;
};

const test_case cases[] = {
    {"ten", 10, 55},
    {"twenty", 20, 6765},
    {"thirty", 30, 832040},
    {"zero", 0, 0},
    {"one", 1, 1},
    {"two", 2, 1},
    {"largest-32-bit", 46, 1836311903},
    {"beyond-32-bit", 47, 2971215073LL},
    {"binet-fails", 71, 308061521170129LL},
    {"largest-64-bit", 92, 7540113804746346429LL},
};

static_assert(fibonacci(10) == 55);

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const long long got = fibonacci(c.n);
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
