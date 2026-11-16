// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

#include "checked_add.hpp"

#include <cstdio>

namespace {

constexpr auto MAX = std::numeric_limits<std::int64_t>::max();
constexpr auto MIN = std::numeric_limits<std::int64_t>::min();

// The function is constexpr, so the compiler can check it too.
static_assert(checked_add(MAX, 0) == MAX);
static_assert(!checked_add(MAX, 1).has_value());

int failures = 0;

void check(const char *label, std::int64_t a, std::int64_t b, std::optional<std::int64_t> want) {
    if (checked_add(a, b) != want) {
        std::fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

} // namespace

int main() {
    check("small", 2, 3, 5);
    check("negative", -7, 4, -3);
    check("beyond-32-bit", 4000000000, 5000000000, 9000000000);
    check("max-plus-zero", MAX, 0, MAX);
    check("max-plus-one", MAX, 1, std::nullopt);
    check("max-plus-max", MAX, MAX, std::nullopt);
    check("min-plus-zero", MIN, 0, MIN);
    check("min-minus-one", MIN, -1, std::nullopt);
    check("min-plus-min", MIN, MIN, std::nullopt);
    check("min-plus-max", MIN, MAX, -1);
    check("max-minus-one", MAX, -1, MAX - 1);
    if (failures > 0) {
        return 1;
    }
    std::printf("all cases pass\n");
    return 0;
}
