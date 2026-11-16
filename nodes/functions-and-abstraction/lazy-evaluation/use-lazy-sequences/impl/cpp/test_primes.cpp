// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

#include "primes.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using Ints = std::vector<int>;

static void check(std::string_view name, std::size_t n, const Ints &want) {
    if (first_primes(n) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("five", 5, Ints{2, 3, 5, 7, 11});
    check("ten", 10, Ints{2, 3, 5, 7, 11, 13, 17, 19, 23, 29});
    check("twenty-five", 25, Ints{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97});
    check("zero", 0, Ints{});
    check("one", 1, Ints{2});
    check("two", 2, Ints{2, 3});
    check("past-a-gap", 31, Ints{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
