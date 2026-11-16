// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

#include "arith.hpp"

#include <cstdio>

namespace {

using Result = std::array<std::int64_t, 6>;

static_assert(arith(7, 2, 3) == Result{9, 5, 14, 3, 1, 343});

int failures = 0;

void check(const char *label, std::int64_t a, std::int64_t b, int n, const Result &want) {
    if (arith(a, b, n) != want) {
        std::fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

} // namespace

int main() {
    check("small", 7, 2, 3, {9, 5, 14, 3, 1, 343});
    check("exact-division", 20, 5, 2, {25, 15, 100, 4, 0, 400});
    check("negative-dividend", -7, 2, 3, {-5, -9, -14, -3, -1, -343});
    check("negative-divisor", 7, -2, 2, {5, 9, -14, -3, 1, 49});
    check("both-negative", -7, -2, 1, {-9, -5, 14, 3, -1, -7});
    check("zero-dividend", 0, 5, 0, {5, -5, 0, 0, 0, 1});
    check("zero-power", 5, 3, 0, {8, 2, 15, 1, 2, 1});
    check("divisor-larger", 3, 7, 1, {10, -4, 21, 0, 3, 3});
    check("largest", 100000, 100000, 3, {200000, 0, 10000000000, 1, 0, 1000000000000000});
    check("smallest", -100000, 1, 3, {-99999, -100001, -100000, -100000, 0, -1000000000000000});
    if (failures > 0) {
        return 1;
    }
    std::printf("all cases pass\n");
    return 0;
}
