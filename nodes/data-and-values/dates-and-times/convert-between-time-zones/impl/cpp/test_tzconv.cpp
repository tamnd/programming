// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

#include <chrono>
#include <cstdio>
#include <stdexcept>
#include <string>

#include "tzconv.hpp"

namespace {

struct test_case {
    const char* name;
    int year;
    unsigned month, day;
    int hour, minute, second;
    const char* from_zone;
    const char* to_zone;
    const char* want;
};

const test_case cases[] = {
    {"new-york-to-utc", 2024, 7, 1, 12, 0, 0, "America/New_York", "UTC", "2024-07-01T16:00:00+00:00"},
    {"utc-to-tokyo", 2024, 1, 15, 0, 0, 0, "UTC", "Asia/Tokyo", "2024-01-15T09:00:00+09:00"},
    {"london-to-new-york", 2024, 1, 15, 12, 0, 0, "Europe/London", "America/New_York", "2024-01-15T07:00:00-05:00"},
    {"date-changes", 2024, 1, 15, 22, 0, 0, "America/Los_Angeles", "Asia/Tokyo", "2024-01-16T15:00:00+09:00"},
    {"same-zone", 2024, 3, 10, 12, 0, 0, "America/New_York", "America/New_York", "2024-03-10T12:00:00-04:00"},
    {"nonexistent-time", 2024, 3, 10, 2, 30, 0, "America/New_York", "UTC", "2024-03-10T07:30:00+00:00"},
    {"ambiguous-time", 2024, 11, 3, 1, 30, 0, "America/New_York", "UTC", "2024-11-03T05:30:00+00:00"},
    {"ambiguous-time-london", 2024, 10, 27, 1, 30, 0, "Europe/London", "UTC", "2024-10-27T00:30:00+00:00"},
    {"dst-dates-differ", 2024, 3, 20, 12, 0, 0, "America/New_York", "Europe/London", "2024-03-20T16:00:00+00:00"},
    {"southern-hemisphere", 2024, 1, 15, 12, 0, 0, "Australia/Sydney", "UTC", "2024-01-15T01:00:00+00:00"},
    {"half-hour-offset", 2024, 1, 15, 12, 0, 0, "Asia/Kolkata", "UTC", "2024-01-15T06:30:00+00:00"},
    {"45-minute-offset", 2024, 1, 15, 12, 0, 0, "UTC", "Asia/Kathmandu", "2024-01-15T17:45:00+05:45"},
    {"half-hour-dst", 2024, 1, 15, 12, 0, 0, "Australia/Lord_Howe", "UTC", "2024-01-15T01:00:00+00:00"},
    {"across-the-date-line", 2024, 1, 15, 12, 0, 0, "Pacific/Kiritimati", "Pacific/Pago_Pago", "2024-01-14T11:00:00-11:00"},
    {"before-skipped-day", 2011, 12, 29, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-29T22:00:00+00:00"},
    {"skipped-day", 2011, 12, 30, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-30T22:00:00+00:00"},
    {"after-skipped-day", 2011, 12, 31, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-30T22:00:00+00:00"},
    {"old-dst-rule", 2006, 3, 15, 12, 0, 0, "America/New_York", "UTC", "2006-03-15T17:00:00+00:00"},
    {"posix-sign", 2024, 1, 15, 12, 0, 0, "UTC", "Etc/GMT+5", "2024-01-15T07:00:00-05:00"},
    {"result-before-epoch", 1970, 1, 1, 0, 0, 0, "UTC", "America/New_York", "1969-12-31T19:00:00-05:00"},
    {"far-future", 2050, 7, 1, 12, 0, 0, "America/New_York", "UTC", "2050-07-01T16:00:00+00:00"},
};

}  // namespace

int main() {
    using namespace std::chrono;
    int failures = 0;
    // The tz data has -10:00 in Pacific/Apia until 2011-12-30 10:00 UTC and
    // then +14:00. libstdc++ of GCC 13 and GCC 15 gives +13:00 from
    // 2011-12-29 00:00 UTC, so the cases for Apia run only with a library
    // that reads this change as the data gives it.
    const bool apia_ok =
        locate_zone("Pacific/Apia")->get_info(sys_days{2011y / December / 29} + 22h).offset == -10h;
    if (!apia_ok) {
        std::printf("skip the cases for Pacific/Apia\n");
    }
    for (const test_case& c : cases) {
        if (!apia_ok && std::string{c.from_zone} == "Pacific/Apia") {
            continue;
        }
        const std::string got = convert_time_zone(c.year, c.month, c.day, c.hour, c.minute, c.second,
                                                  c.from_zone, c.to_zone);
        if (got != c.want) {
            std::printf("FAIL %s: %s\n", c.name, got.c_str());
            ++failures;
        }
    }
    // choose::earliest gives the transition itself for a time in a gap.
    const local_seconds gap = local_days{2024y / March / 10} + 2h + 30min;
    const zoned_time<seconds> at{"America/New_York", gap, choose::earliest};
    if (at.get_sys_time() != sys_days{2024y / March / 10} + 7h) {
        std::printf("FAIL choose::earliest in a gap\n");
        ++failures;
    }
    try {
        convert_time_zone(2024, 1, 1, 0, 0, 0, "Mars/Olympus", "UTC");
        std::printf("FAIL unknown zone\n");
        ++failures;
    } catch (const std::runtime_error&) {
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
