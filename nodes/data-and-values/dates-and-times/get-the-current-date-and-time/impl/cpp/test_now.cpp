// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>

#include "now.hpp"

namespace {

struct test_case {
    const char* name;
    std::int64_t epoch_ns;
    const char* want;
};

const test_case cases[] = {
    {"epoch", INT64_C(0), "1970-01-01T00:00:00.000Z"},
    {"billennium", INT64_C(1000000000000000000), "2001-09-09T01:46:40.000Z"},
    {"with-milliseconds", INT64_C(1710054000123456789), "2024-03-10T07:00:00.123Z"},
    {"leap-day", INT64_C(1709208000000000000), "2024-02-29T12:00:00.000Z"},
    {"truncate-not-round", INT64_C(999999), "1970-01-01T00:00:00.000Z"},
    {"one-ns-before-epoch", INT64_C(-1), "1969-12-31T23:59:59.999Z"},
    {"one-ms-before-epoch", INT64_C(-1000000), "1969-12-31T23:59:59.999Z"},
    {"just-over-one-ms-before-epoch", INT64_C(-1000001), "1969-12-31T23:59:59.998Z"},
    {"last-ms-of-a-year", INT64_C(1704067199999999999), "2023-12-31T23:59:59.999Z"},
    {"year-2000-is-leap", INT64_C(951782400000000000), "2000-02-29T00:00:00.000Z"},
    {"year-2100-is-not-leap", INT64_C(4107542399999999999), "2100-02-28T23:59:59.999Z"},
    {"after-int32-seconds", INT64_C(2147483648000000000), "2038-01-19T03:14:08.000Z"},
    {"int64-max", INT64_C(9223372036854775807), "2262-04-11T23:47:16.854Z"},
    {"int64-min", INT64_MIN, "1677-09-21T00:12:43.145Z"},
};

int calls = 0;

// A clock with one fixed reading that counts its calls.
struct fake_clock {
    static std::chrono::sys_time<std::chrono::nanoseconds> now() {
        ++calls;
        return std::chrono::sys_time<std::chrono::nanoseconds>{std::chrono::nanoseconds{INT64_C(1710054000123456789)}};
    }
};

}  // namespace

int main() {
    int failed = 0;
    for (const auto& c : cases) {
        std::string got = utc_timestamp(c.epoch_ns);
        if (got != c.want) {
            std::printf("FAIL %s: got %s, want %s\n", c.name, got.c_str(), c.want);
            failed++;
        }
    }

    std::string fake = now_utc<fake_clock>();
    if (fake != "2024-03-10T07:00:00.123Z" || calls != 1) {
        std::printf("FAIL fake clock: got %s, %d calls\n", fake.c_str(), calls);
        failed++;
    }

    std::string real = now_utc();
    if (real.size() != 24 || real[10] != 'T' || real[19] != '.' || real[23] != 'Z' || std::stoi(real.substr(0, 4)) < 2024) {
        std::printf("FAIL real clock: got %s\n", real.c_str());
        failed++;
    }

    if (failed == 0) {
        std::printf("ok\n");
    }
    return failed != 0;
}
