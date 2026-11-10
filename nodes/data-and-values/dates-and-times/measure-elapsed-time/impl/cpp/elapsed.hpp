// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

#ifndef ELAPSED_HPP
#define ELAPSED_HPP

#include <chrono>
#include <cstdint>
#include <string>
#include <utility>

// Runs work once and returns the time it took on Clock. steady_clock is monotonic, and it is not related to the
// wall clock. A test can give another type with a static now().
template <typename Clock = std::chrono::steady_clock, typename Work>
typename Clock::duration measure(Work&& work) {
    static_assert(Clock::is_steady, "measure needs a clock that does not go back");
    auto start = Clock::now();
    std::forward<Work>(work)();
    return Clock::now() - start;
}

// Returns end_ns - start_ns as "M.UUU ms", with the rest under one microsecond dropped.
std::string elapsed_ms(std::int64_t start_ns, std::int64_t end_ns);

#endif
