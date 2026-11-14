// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

#include <chrono>
#include <cstdio>
#include <vector>

#include "sleep.hpp"

namespace {

struct test_case {
    const char* name;
    long long duration_ns, start_ns;
    std::vector<long long> readings, want;
};

const test_case cases[] = {
    {"one-call", 1000000LL, 0LL, {1000000LL}, {1000000LL}},
    {"woke-late", 1000000LL, 0LL, {1080000LL}, {1000000LL}},
    {"woke-early", 1000000LL, 0LL, {600000LL, 1000000LL}, {1000000LL, 400000LL}},
    {"one-second", 1000000000LL, 5000000000LL, {6000050000LL}, {1000000000LL}},
    {"zero", 0LL, 0LL, {}, {}},
    {"negative", -5000000LL, 0LL, {}, {}},
    {"most-negative", -4611686018427387904LL, 0LL, {}, {}},
    {"several-early-wakes", 10000000LL, 0LL, {2000000LL, 5000000LL, 9999999LL, 10000000LL}, {10000000LL, 8000000LL, 5000000LL, 1LL}},
    {"no-progress", 1000000LL, 0LL, {0LL, 1000000LL}, {1000000LL, 1000000LL}},
    {"exact-deadline", 3000000LL, 100LL, {3000100LL}, {3000000LL}},
    {"one-ns-short", 3000000LL, 0LL, {2999999LL, 3000000LL}, {3000000LL, 1LL}},
    {"extra-readings", 1000000LL, 0LL, {1000000LL, 5000000LL}, {1000000LL}},
    {"negative-origin", 2000000000LL, -5000000000LL, {-4000000000LL, -3000000000LL}, {2000000000LL, 1000000000LL}},
    {"origin-after-100-days", 1500000LL, 8640000000000000LL, {8640000001000000LL, 8640000001500000LL}, {1500000LL, 500000LL}},
    {"one-nanosecond", 1LL, 0LL, {1000000LL}, {1LL}},
    {"sub-millisecond", 1500000LL, 0LL, {1000000LL, 1500000LL}, {1500000LL, 500000LL}},
    {"one-hour", 3600000000000LL, 0LL, {3600000000000LL}, {3600000000000LL}},
    {"thirty-days", 2592000000000000LL, 0LL, {2147483647000000LL, 2592000000000000LL}, {2592000000000000LL, 444516353000000LL}},
};

}  // namespace

int main() {
    using std::chrono::steady_clock;
    int failed = 0;
    for (const test_case& c : cases) {
        if (sleep_requests(c.duration_ns, c.start_ns, c.readings) != c.want) {
            std::printf("FAIL %s\n", c.name);
            failed = 1;
        }
    }
    auto start = steady_clock::now();
    sleep_for_ns(20'000'000);
    if (steady_clock::now() - start < std::chrono::milliseconds{20}) {
        std::printf("FAIL sleep_for_ns(20 ms) returned early\n");
        failed = 1;
    }
    for (const long long ns : {0LL, -5'000'000LL}) {
        start = steady_clock::now();
        sleep_for_ns(ns);
        if (steady_clock::now() - start >= std::chrono::seconds{1}) {
            std::printf("FAIL sleep_for_ns(%lld) did not return at once\n", ns);
            failed = 1;
        }
    }
    if (failed != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
