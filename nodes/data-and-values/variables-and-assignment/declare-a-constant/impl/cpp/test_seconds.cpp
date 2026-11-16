// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

#include <cstdio>

#include "seconds.hpp"

namespace {

struct test_case {
    const char* name;
    long long days;
    long long want;
};

const test_case cases[] = {
    {"one-day", 1, 86400},
    {"one-week", 7, 604800},
    {"one-year", 365, 31536000},
    {"zero", 0, 0},
    {"two-days", 2, 172800},
    {"leap-year", 366, 31622400},
    {"last-32-bit-day", 24855, 2147472000},
    {"past-32-bits", 24856, 2147558400LL},
    {"million-days", 1000000, 86400000000LL},
};

static_assert(seconds_in(7) == 604800);

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const long long got = seconds_in(c.days);
        if (got != c.want) {
            std::printf("FAIL %s: got %lld\n", c.name, got);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
