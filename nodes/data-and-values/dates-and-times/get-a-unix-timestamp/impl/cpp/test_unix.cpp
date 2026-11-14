// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <format>
#include <string>

#include "unix.hpp"

namespace {

struct test_case {
    const char* name;
    int year, month, day, hour, minute, second, offset;
    long long want;
};

const test_case cases[] = {
    {"epoch", 1970, 1, 1, 0, 0, 0, 0, 0LL},
    {"billennium", 2001, 9, 9, 1, 46, 40, 0, 1000000000LL},
    {"negative-offset", 2024, 3, 10, 2, 0, 0, -300, 1710054000LL},
    {"positive-offset", 2024, 3, 10, 16, 0, 0, 540, 1710054000LL},
    {"one-second-before-epoch", 1969, 12, 31, 23, 59, 59, 0, -1LL},
    {"offset-moves-before-epoch", 1970, 1, 1, 0, 0, 0, 60, -3600LL},
    {"offset-moves-to-next-year", 2023, 12, 31, 19, 0, 0, -300, 1704067200LL},
    {"offset-of-45-minutes", 2024, 1, 1, 5, 45, 0, 345, 1704067200LL},
    {"offset-of-minus-9-30", 2023, 12, 31, 14, 30, 0, -570, 1704067200LL},
    {"offset-plus-14", 1970, 1, 1, 14, 0, 0, 840, 0LL},
    {"offset-minus-12", 1969, 12, 31, 12, 0, 0, -720, 0LL},
    {"leap-day", 2024, 2, 29, 0, 0, 0, 0, 1709164800LL},
    {"year-2000-is-leap", 2000, 2, 29, 0, 0, 0, 0, 951782400LL},
    {"year-1900-is-not-leap", 1900, 3, 1, 0, 0, 0, 0, -2203891200LL},
    {"int32-max", 2038, 1, 19, 3, 14, 7, 0, 2147483647LL},
    {"after-int32-max", 2038, 1, 19, 3, 14, 8, 0, 2147483648LL},
    {"int32-min", 1901, 12, 13, 20, 45, 52, 0, -2147483648LL},
    {"year-99-is-not-1999", 99, 12, 31, 23, 59, 59, 0, -59011459201LL},
    {"first-second-of-year-1", 1, 1, 1, 0, 0, 0, 0, -62135596800LL},
    {"last-second-of-year-9999", 9999, 12, 31, 23, 59, 59, 0, 253402300799LL},
};

int failures = 0;

void expect(bool ok, const char* what, const char* name) {
    if (!ok) {
        std::printf("FAIL %s: %s\n", name, what);
        ++failures;
    }
}

}  // namespace

int main() {
    using namespace std::chrono;
    for (const auto& c : cases) {
        expect(unix_timestamp(c.year, c.month, c.day, c.hour, c.minute, c.second, c.offset) == c.want,
               "unix_timestamp", c.name);
        const sys_seconds back{seconds{c.want}};
        const auto local = back + minutes{c.offset};
        const std::string text = std::format("{:%FT%T}", local);
        const std::string want = std::format("{:04}-{:02}-{:02}T{:02}:{:02}:{:02}", c.year, c.month, c.day, c.hour,
                                             c.minute, c.second);
        expect(text == want, "back to a date", c.name);
    }

    const auto secs = now_unix_seconds();
    const auto millis = now_unix_millis();
    expect(secs >= 1704067200, "now_unix_seconds is in 2024 or later", "now");
    const auto diff = millis / 1000 - secs;
    expect(diff >= 0 && diff <= 1, "seconds and milliseconds agree", "now");

    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
