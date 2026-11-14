// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

#ifndef CONVERT_TIME_ZONE_HPP
#define CONVERT_TIME_ZONE_HPP

#include <chrono>
#include <format>
#include <string>
#include <string_view>

// Returns the instant of a local time in zone. A time in a gap moves forward
// by the length of the gap, and a time in an overlap gives the earlier of its
// two instants. choose::earliest maps a time in a gap to the transition
// itself, so the offset before the gap is used here.
inline std::chrono::sys_seconds local_to_instant(const std::chrono::time_zone* zone,
                                                 std::chrono::local_seconds local) {
    const std::chrono::local_info info = zone->get_info(local);
    // For a unique or nonexistent time, first is the offset before; for an
    // ambiguous time, first is the earlier offset.
    return std::chrono::sys_seconds{local.time_since_epoch() - info.first.offset};
}

// Returns the local time in to_zone for a local time in from_zone, as
// YYYY-MM-DDTHH:MM:SS+HH:MM. locate_zone throws std::runtime_error for an
// unknown name.
inline std::string convert_time_zone(int year, unsigned month, unsigned day, int hour, int minute,
                                     int second, std::string_view from_zone,
                                     std::string_view to_zone) {
    using namespace std::chrono;
    const local_seconds local = local_days{std::chrono::year{year} / month / day} + hours{hour} +
                                minutes{minute} + seconds{second};
    const sys_seconds instant = local_to_instant(locate_zone(from_zone), local);
    const zoned_time<seconds> there{locate_zone(to_zone), instant};
    return std::format("{:%FT%T%Ez}", there);
}

#endif
