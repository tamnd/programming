// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

#include <chrono>
#include <cstdio>
#include <string>

#include "duration.hpp"

namespace {

struct test_case {
    const char* name;
    long long seconds;
    const char* want;
};

const test_case cases[] = {
    {"zero", 0LL, "00:00:00"},
    {"seconds-only", 59LL, "00:00:59"},
    {"one-minute", 60LL, "00:01:00"},
    {"one-hour", 3600LL, "01:00:00"},
    {"hours-minutes-seconds", 3723LL, "01:02:03"},
    {"just-under-an-hour", 3599LL, "00:59:59"},
    {"just-under-a-day", 86399LL, "23:59:59"},
    {"one-day", 86400LL, "24:00:00"},
    {"more-than-a-day", 90061LL, "25:01:01"},
    {"hundred-hours", 360000LL, "100:00:00"},
    {"one-week", 604800LL, "168:00:00"},
    {"one-year", 31536000LL, "8760:00:00"},
    {"minus-one-second", -1LL, "-00:00:01"},
    {"negative-minute", -61LL, "-00:01:01"},
    {"negative-hours", -3723LL, "-01:02:03"},
    {"negative-day", -90061LL, "-25:01:01"},
    {"beyond-go-duration", 10000000000LL, "2777777:46:40"},
    {"largest", 1000000000000000LL, "277777777777:46:40"},
    {"most-negative", -1000000000000000LL, "-277777777777:46:40"},
};

}  // namespace

int main() {
    int failures = 0;
    for (const test_case& c : cases) {
        const std::string got = format_duration(c.seconds);
        if (got != c.want) {
            std::printf("FAIL %s: %s\n", c.name, got.c_str());
            ++failures;
        }
    }
    // hh_mm_ss gives the same fields.
    const std::chrono::hh_mm_ss<std::chrono::seconds> h{std::chrono::seconds{-90061}};
    if (!h.is_negative() || h.hours().count() != 25 || h.minutes().count() != 1 || h.seconds().count() != 1) {
        std::printf("FAIL hh_mm_ss\n");
        ++failures;
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
