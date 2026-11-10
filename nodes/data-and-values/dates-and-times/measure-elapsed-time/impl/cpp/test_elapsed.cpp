// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "elapsed.hpp"

namespace {

struct test_case {
    const char* name;
    std::int64_t start_ns, end_ns;
    const char* want;
};

const test_case cases[] = {
    {"one-and-a-half-ms", INT64_C(1000000000), INT64_C(1001500000), "1.500 ms"},
    {"seconds", INT64_C(0), INT64_C(2345678901), "2345.678 ms"},
    {"under-one-ms", INT64_C(50000), INT64_C(750000), "0.700 ms"},
    {"zero", INT64_C(123456789), INT64_C(123456789), "0.000 ms"},
    {"under-one-microsecond", INT64_C(0), INT64_C(999), "0.000 ms"},
    {"truncate-not-round", INT64_C(0), INT64_C(1999999), "1.999 ms"},
    {"leading-zero-in-fraction", INT64_C(0), INT64_C(1005000), "1.005 ms"},
    {"exact-ms", INT64_C(0), INT64_C(7000000), "7.000 ms"},
    {"one-hour", INT64_C(0), INT64_C(3600000000000), "3600000.000 ms"},
    {"negative-readings", INT64_C(-5000000), INT64_C(-3500000), "1.500 ms"},
    {"across-zero", INT64_C(-1000000), INT64_C(500000), "1.500 ms"},
    {"beyond-2-pow-53", INT64_C(9007199254740993), INT64_C(9007199255741992), "1.000 ms"},
    {"near-int64-max", INT64_C(9223372035854775807), INT64_C(9223372036854775807), "1000.000 ms"},
    {"near-int64-min", INT64_MIN, INT64_C(-9223372035854775808), "1000.000 ms"},
};

std::string calls;

// A clock that returns fixed readings in order and logs each call.
struct fake_clock {
    using rep = std::int64_t;
    using period = std::nano;
    using duration = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<fake_clock>;
    static constexpr bool is_steady = true;
    static inline std::vector<rep> readings;
    static time_point now() {
        calls += "clock ";
        rep r = readings.front();
        readings.erase(readings.begin());
        return time_point(duration(r));
    }
};

}  // namespace

int main() {
    int failed = 0;
    for (const test_case& c : cases) {
        std::string got = elapsed_ms(c.start_ns, c.end_ns);
        if (got != c.want) {
            std::fprintf(stderr, "FAIL %s: got \"%s\"\n", c.name, got.c_str());
            failed++;
        }
    }

    fake_clock::readings = {5'000'000, 6'500'000};
    auto got = measure<fake_clock>([] { calls += "work "; });
    if (got != std::chrono::microseconds(1'500) || calls != "clock work clock ") {
        std::fprintf(stderr, "FAIL fake clock: calls \"%s\"\n", calls.c_str());
        failed++;
    }

    calls.clear();
    if (measure([] { calls += "work "; }) < std::chrono::steady_clock::duration::zero() || calls != "work ") {
        std::fprintf(stderr, "FAIL steady clock\n");
        failed++;
    }

    if (failed > 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
