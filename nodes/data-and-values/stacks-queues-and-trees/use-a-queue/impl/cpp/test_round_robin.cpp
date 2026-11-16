// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

#include "round_robin.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view label, const std::vector<std::int32_t> &work, std::int32_t slice,
                  const std::vector<std::size_t> &want) {
    if (finish_order(work, slice) != want) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    check("example", {3, 1, 2}, 1, {1, 2, 0});
    check("slice-two", {4, 1, 3, 2}, 2, {1, 3, 0, 2});
    check("empty", {}, 3, {});
    check("one", {7}, 2, {0});
    check("big-slice", {3, 1, 2}, 10, {0, 1, 2});
    check("slice-one", {2, 2, 1}, 1, {2, 0, 1});
    check("equal-work", {2, 2, 2}, 1, {0, 1, 2});
    check("long-job-first", {10, 1, 1}, 3, {1, 2, 0});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
