// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

#include "grade.hpp"

#include <iostream>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, int score, std::string_view want) {
    if (grade(score) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("ninety-five", 95, "A");
    check("eighty-five", 85, "B");
    check("seventy-five", 75, "C");
    check("sixty-five", 65, "D");
    check("forty", 40, "F");
    check("hundred", 100, "A");
    check("zero", 0, "F");
    check("ninety", 90, "A");
    check("eighty-nine", 89, "B");
    check("sixty", 60, "D");
    check("fifty-nine", 59, "F");
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
