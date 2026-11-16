// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

#include <climits>
#include <cstdio>
#include <vector>

#include "sumprod.hpp"

namespace {

struct test_case {
    const char* name;
    std::vector<long long> items;
    long long sum, product;
};

const test_case cases[] = {
    {"small", {1, 2, 3, 4}, 10, 24},
    {"mixed", {3, -2, 5}, 6, -30},
    {"empty", {}, 0, 1},
    {"single", {7}, 7, 7},
    {"with-zero", {5, 0, 9}, 14, 0},
    {"two-negatives", {-2, -3}, -5, 6},
    {"three-negatives", {-1, -2, -3}, -6, -6},
    {"ones", {1, 1, 1}, 3, 1},
    {"large-product", {3037000499LL, 3037000499LL}, 6074000998LL, 9223372030926249001LL},
    {"to-the-limit", {9223372036854775806LL, 1}, 9223372036854775807LL, 9223372036854775806LL},
    {"beyond-double", {9007199254740993LL}, 9007199254740993LL, 9007199254740993LL},
    {"min", {LLONG_MIN}, LLONG_MIN, LLONG_MIN},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const auto [sum, product] = sum_and_product(c.items);
        if (sum != c.sum || product != c.product) {
            std::printf("FAIL %s: (%lld, %lld)\n", c.name, sum, product);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
