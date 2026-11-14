// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

#pragma once

#include <chrono>
#include <cstdint>

// Returns the seconds since 1970-01-01T00:00:00Z of a local time with the
// given UTC offset in minutes.
inline std::int64_t unix_timestamp(int year, int month, int day, int hour, int minute, int second,
                                   int offset_minutes) {
    using namespace std::chrono;
    const sys_days date = std::chrono::year{year} / month / day;
    const sys_seconds utc = date + hours{hour} + minutes{minute - offset_minutes} + seconds{second};
    return utc.time_since_epoch().count();
}

// Returns the current Unix time in whole seconds, rounded toward the past.
inline std::int64_t now_unix_seconds() {
    using namespace std::chrono;
    return floor<seconds>(system_clock::now()).time_since_epoch().count();
}

// Returns the current Unix time in whole milliseconds, rounded toward the past.
inline std::int64_t now_unix_millis() {
    using namespace std::chrono;
    return floor<milliseconds>(system_clock::now()).time_since_epoch().count();
}
