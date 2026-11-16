// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

#include "id_generator.hpp"

#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int calls, const std::vector<long> &want) {
    if (singleton_report(calls) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    std::vector<IdGenerator *> got(8);
    {
        std::vector<std::jthread> threads;
        for (auto &p : got) {
            threads.emplace_back([&p] { p = &IdGenerator::instance(); });
        }
    }
    for (auto *p : got) {
        if (p != &IdGenerator::instance()) {
            std::cerr << "case threads failed\n";
            failures++;
        }
    }

    check("three-calls", 3, {2, 1, 1});
    check("ten-calls", 10, {9, 1, 1});
    check("single-call", 1, {0, 1, 1});
    check("pair", 2, {1, 1, 1});
    check("limit", 1000000, {999999, 1, 1});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
