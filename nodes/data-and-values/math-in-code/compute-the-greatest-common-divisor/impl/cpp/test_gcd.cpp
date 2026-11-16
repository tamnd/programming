// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

#include <cstdio>

#include "gcd.hpp"

namespace {

struct test_case {
    const char* name;
    long long a, b, gcd, lcm;
};

const test_case cases[] = {
    {"common-factor", 12, 18, 6, 36},
    {"coprime", 17, 5, 1, 85},
    {"euclid-example", 48, 180, 12, 720},
    {"zero-and-number", 0, 7, 7, 0},
    {"both-zero", 0, 0, 0, 0},
    {"negative", -12, 18, 6, 36},
    {"both-negative", -4, -6, 2, 12},
    {"equal", 9, 9, 9, 9},
    {"one", 1, 1000000007, 1, 1000000007},
    {"multiple", 5, 25, 5, 25},
    {"fibonacci", 832040, 514229, 1, 427859097160LL},
    {"big-lcm", 3000000000000000000LL, 6000000000000000000LL, 3000000000000000000LL, 6000000000000000000LL},
    {"largest", 9223372036854775807LL, -9223372036854775807LL, 9223372036854775807LL, 9223372036854775807LL},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const auto want = std::pair{c.gcd, c.lcm};
        if (gcd_and_lcm(c.a, c.b) != want || gcd_and_lcm(c.b, c.a) != want) {
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
