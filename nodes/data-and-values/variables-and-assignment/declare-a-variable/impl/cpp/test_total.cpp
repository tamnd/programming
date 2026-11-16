// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

#include <cstdio>
#include <vector>

#include "total.hpp"

namespace {

struct test_case {
    const char* name;
    std::vector<long long> values;
    long long want;
};

const test_case cases[] = {
    {"one-two-three", {1, 2, 3}, 6},
    {"four-values", {10, 20, 30, 40}, 100},
    {"mixed-signs", {5, -2, 7}, 10},
    {"empty", {}, 0},
    {"single", {42}, 42},
    {"all-negative", {-1, -2, -3}, -6},
    {"zeros", {0, 0, 0}, 0},
    {"cancel-out", {7, -7}, 0},
    {"past-32-bits", {4000000000LL, 5000000000LL}, 9000000000LL},
    {"eight-values", {1, 1, 1, 1, 1, 1, 1, 1}, 8},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const long long got = total(c.values);
        if (got != c.want) {
            std::printf("FAIL %s: got %lld\n", c.name, got);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
