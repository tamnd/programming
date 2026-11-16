// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

#include <cstddef>
#include <cstdio>
#include <vector>

#include "swap.hpp"

namespace {

struct test_case {
    const char* name;
    std::vector<long long> xs;
    std::size_t i;
    std::size_t j;
    std::vector<long long> want;
};

const test_case cases[] = {
    {"first-and-last", {1, 2, 3}, 0, 2, {3, 2, 1}},
    {"middle-pair", {10, 20, 30, 40}, 1, 2, {10, 30, 20, 40}},
    {"two-items", {5, 7}, 0, 1, {7, 5}},
    {"single-item", {9}, 0, 0, {9}},
    {"same-index", {1, 2, 3}, 1, 1, {1, 2, 3}},
    {"higher-index-first", {1, 2, 3, 4}, 3, 0, {4, 2, 3, 1}},
    {"equal-values", {4, 4}, 0, 1, {4, 4}},
    {"negative-values", {-1, 0, 1}, 0, 2, {1, 0, -1}},
    {"int32-extremes", {2147483647, -2147483648LL}, 0, 1, {-2147483648LL, 2147483647}},
    {"eight-items", {1, 2, 3, 4, 5, 6, 7, 8}, 2, 6, {1, 2, 7, 4, 5, 6, 3, 8}},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (swap_items(c.xs, c.i, c.j) != c.want) {
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
