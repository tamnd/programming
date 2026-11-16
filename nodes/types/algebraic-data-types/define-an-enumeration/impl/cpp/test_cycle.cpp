// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

#include "cycle.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

using Names = std::vector<std::string>;

static void check(std::string_view label, std::string_view start, int steps, const Names &want) {
    if (cycle(start, steps) != want) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    check("weekdays", "monday", 3, {"monday", "tuesday", "wednesday"});
    check("wrap", "friday", 4, {"friday", "saturday", "sunday", "monday"});
    check("zero-steps", "monday", 0, {});
    check("more-than-a-week", "sunday", 9,
          {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday"});
    check("unknown", "funday", 3, {});
    check("wrong-case", "Friday", 2, {});
    check("number", "3", 2, {});
    check("empty-name", "", 1, {});
    if (static_cast<int>(Day::Friday) != 4 || to_string(Day::Friday) != "friday" || parse_day("friday") != Day::Friday) {
        std::cerr << "case to-and-from failed\n";
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
