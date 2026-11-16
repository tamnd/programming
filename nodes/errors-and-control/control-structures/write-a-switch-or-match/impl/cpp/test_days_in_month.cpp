// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

#include "days_in_month.hpp"

#include <iostream>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, int month, int want) {
    if (days_in_month(month) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("january", 1, 31);
    check("february", 2, 28);
    check("april", 4, 30);
    check("july", 7, 31);
    check("november", 11, 30);
    check("december", 12, 31);
    check("zero", 0, 0);
    check("thirteen", 13, 0);
    check("minus-one", -1, 0);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
