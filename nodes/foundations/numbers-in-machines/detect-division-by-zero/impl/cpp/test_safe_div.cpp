// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

#include "safe_div.hpp"

#include <cstdio>

namespace {

constexpr auto MAX = std::numeric_limits<std::int64_t>::max();
constexpr auto MIN = std::numeric_limits<std::int64_t>::min();

static_assert(safe_div(7, 2) == 3);
static_assert(!safe_div(7, 0).has_value());

int failures = 0;

void check(const char *label, std::int64_t a, std::int64_t b, std::optional<std::int64_t> want) {
    if (safe_div(a, b) != want) {
        std::fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

} // namespace

int main() {
    check("exact", 12, 4, 3);
    check("remainder-dropped", 7, 2, 3);
    check("zero-dividend", 0, 5, 0);
    check("zero-divisor", 5, 0, std::nullopt);
    check("negative-by-zero", -5, 0, std::nullopt);
    check("zero-by-zero", 0, 0, std::nullopt);
    check("negative-dividend", -7, 2, -3);
    check("negative-divisor", 7, -2, -3);
    check("both-negative", -7, -2, 3);
    check("min-by-minus-one", MIN, -1, std::nullopt);
    check("min-by-one", MIN, 1, MIN);
    check("max-by-minus-one", MAX, -1, -MAX);
    check("min-by-max", MIN, MAX, -1);
    if (failures > 0) {
        return 1;
    }
    std::printf("all cases pass\n");
    return 0;
}
