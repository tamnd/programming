// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

#include "halvings.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int n, const std::vector<int> &want) {
    if (halvings(n) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("rosetta", 1024, std::vector<int>{1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1});
    check("ten", 10, std::vector<int>{10, 5, 2, 1});
    check("zero", 0, std::vector<int>{});
    check("one", 1, std::vector<int>{1});
    check("odd", 7, std::vector<int>{7, 3, 1});
    check("limit", 2147483647, std::vector<int>{2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
