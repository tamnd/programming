// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include "meanmedian.hpp"

namespace {

struct test_case {
    const char* name;
    std::vector<long long> items;
    double mean, median;
};

const test_case cases[] = {
    {"odd", {3, 1, 2}, 2.0, 2.0},
    {"even", {4, 1, 3, 2}, 2.5, 2.5},
    {"skewed", {1, 2, 3, 4, 100}, 22.0, 3.0},
    {"single", {5}, 5.0, 5.0},
    {"two-items", {1, 2}, 1.5, 1.5},
    {"negative", {-3, -1, -2}, -2.0, -2.0},
    {"unsorted-even", {10, 2, 8, 4}, 6.0, 6.0},
    {"duplicates", {1, 1, 1, 5}, 2.0, 1.0},
    {"not-an-integer", {1, 2, 2}, 1.6666666666666667, 2.0},
    {"large", {9223372036854775807LL, 9223372036854775807LL}, 9223372036854775808.0, 9223372036854775808.0},
};

bool close_to(double got, double want) {
    return std::abs(got - want) <= std::max(1e-9, 1e-12 * std::abs(want));
}

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const auto [mean, median] = mean_and_median(c.items);
        if (!close_to(mean, c.mean) || !close_to(median, c.median)) {
            std::printf("FAIL %s: (%.17g, %.17g)\n", c.name, mean, median);
            ++failures;
        }
    }
    try {
        mean_and_median({});
        std::printf("FAIL empty\n");
        ++failures;
    } catch (const std::invalid_argument&) {
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
