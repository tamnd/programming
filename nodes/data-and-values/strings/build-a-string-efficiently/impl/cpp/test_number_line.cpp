// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

#include "number_line.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, int n, std::string_view want) {
    std::string got = number_line(n);
    if (got != want) {
        std::cerr << "case " << name << ": got [" << got << "], want [" << want << "]\n";
        failures++;
    }
}

// check_long checks the length, the start and the end of a long result.
static void check_long(std::string_view name, int n, std::size_t size, std::string_view end) {
    std::string got = number_line(n);
    if (got.size() != size || !got.starts_with("1,2,3,") || !got.ends_with(end)) {
        std::cerr << "case " << name << ": got a length of " << got.size() << "\n";
        failures++;
    }
}

int main() {
    check("zero", 0, "");
    check("one", 1, "1");
    check("three", 3, "1,2,3");
    check("five", 5, "1,2,3,4,5");
    check("seven", 7, "1,2,3,4,5,6,7");
    check("nine", 9, "1,2,3,4,5,6,7,8,9");
    check("ten", 10, "1,2,3,4,5,6,7,8,9,10");
    check_long("hundred", 100, 291, ",99,100");
    check_long("hundred-and-one", 101, 295, ",100,101");
    check_long("thousand", 1000, 3892, ",999,1000");
    check_long("large", 100000, 588894, ",99999,100000");
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
