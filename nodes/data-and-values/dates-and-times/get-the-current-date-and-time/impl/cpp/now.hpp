// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

#ifndef NOW_HPP
#define NOW_HPP

#include <chrono>
#include <cstdint>
#include <format>
#include <string>

// Returns the UTC date and time of epoch_ns as "YYYY-MM-DDTHH:MM:SS.mmmZ".
inline std::string utc_timestamp(std::int64_t epoch_ns) {
    using namespace std::chrono;
    // floor goes toward the past. duration_cast and time_point_cast go toward zero.
    sys_time<milliseconds> ms = floor<milliseconds>(sys_time<nanoseconds>{nanoseconds{epoch_ns}});
    // %T writes as many digits of the fraction as the duration has: three for milliseconds.
    return std::format("{:%FT%TZ}", ms);
}

// Reads Clock once and returns the UTC date and time of the reading. Since C++20 the epoch of system_clock is
// 1970-01-01T00:00:00Z without leap seconds. A test can give another type with a static now().
template <typename Clock = std::chrono::system_clock>
std::string now_utc() {
    using namespace std::chrono;
    return utc_timestamp(duration_cast<nanoseconds>(Clock::now().time_since_epoch()).count());
}

#endif
