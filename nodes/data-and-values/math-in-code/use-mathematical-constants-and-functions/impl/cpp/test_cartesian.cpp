// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "cartesian.hpp"

namespace {

struct test_case {
    const char* name;
    double r, degrees, x, y;
};

const test_case cases[] = {
    {"east", 1.0, 0.0, 1.0, 0.0},
    {"north", 2.0, 90.0, 0.0, 2.0},
    {"west", 2.0, 180.0, -2.0, 0.0},
    {"diagonal", 1.0, 45.0, 0.7071067811865476, 0.7071067811865476},
    {"thirty-degrees", 2.0, 30.0, 1.7320508075688772, 1.0},
    {"zero-radius", 0.0, 123.0, 0.0, 0.0},
    {"south", 1.0, 270.0, 0.0, -1.0},
    {"negative-angle", 1.0, -90.0, 0.0, -1.0},
    {"full-turn", 1.0, 360.0, 1.0, 0.0},
    {"two-turns", 3.0, 720.0, 3.0, 0.0},
    {"negative-radius", -1.0, 0.0, -1.0, 0.0},
    {"large-radius", 1000000.0, 60.0, 500000.0, 866025.4037844386},
};

bool close_to(double got, double want) {
    return std::abs(got - want) <= std::max(1e-9, 1e-12 * std::abs(want));
}

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const auto [x, y] = to_cartesian(c.r, c.degrees);
        if (!close_to(x, c.x) || !close_to(y, c.y)) {
            std::printf("FAIL %s: (%.17g, %.17g)\n", c.name, x, y);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
