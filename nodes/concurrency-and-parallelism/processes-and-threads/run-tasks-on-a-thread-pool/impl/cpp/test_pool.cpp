// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

#include "pool.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, std::size_t workers, const std::vector<std::int32_t>& numbers,
                  const std::vector<std::int32_t>& want) {
    if (pool::steps_on_pool(workers, numbers) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("four-jobs", 2, {1, 2, 3, 6}, {0, 1, 7, 8});
    check("long-runs", 3, {27, 97, 871}, {111, 118, 178});
    check("empty", 2, {}, {});
    check("one-worker", 1, {7, 8, 9}, {16, 3, 19});
    check("more-workers-than-jobs", 8, {5, 6}, {5, 8});
    check("duplicates", 3, {27, 27, 27, 1}, {111, 111, 111, 0});
    check("wide-intermediate", 2, {113383, 837799}, {247, 524});
    std::vector<std::int32_t> many, want;
    for (std::int32_t n = 1; n <= 200; n++) {
        many.push_back(n);
        want.push_back(pool::collatz_steps(n));
    }
    check("many", 4, many, want);
    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
