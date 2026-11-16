// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

#include "tls.hpp"

#include <cstdint>
#include <iostream>
#include <numeric>
#include <string_view>
#include <thread>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::int32_t>& counts) {
    if (tls::count_per_thread(counts) != counts) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three-threads", {1, 2, 3});
    check("uneven", {10, 0, 5, 1000});
    check("empty", {});
    check("zero", {0});
    check("one-thread", {7});
    check("equal-counts", {100, 100, 100, 100});
    std::vector<std::int32_t> many(16);
    std::iota(many.begin(), many.end(), 1);
    check("many-threads", many);
    check("large", {100000, 100000});

    // Run in a new thread, so that the counter of the main thread does not matter.
    std::int32_t seen = -1;
    std::thread caller([&seen] {
        for (int k = 0; k < 3; k++) {
            tls::bump();
        }
        tls::count_per_thread({5, 6});
        seen = tls::current();
    });
    caller.join();
    if (seen != 3) {
        std::cerr << "case caller-counter failed: got " << seen << ", want 3\n";
        failures++;
    }
    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
