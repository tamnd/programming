// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

#ifndef DAY_OF_WEEK_HPP
#define DAY_OF_WEEK_HPP

#include <chrono>
#include <utility>

// Returns the ISO 8601 day of the week: 1 for Monday to 7 for Sunday.
// weekday::c_encoding() has Sunday = 0, and iso_encoding() has Sunday = 7.
inline int day_of_week(int y, int m, int d) {
    using namespace std::chrono;
    const sys_days date{year{y} / month{static_cast<unsigned>(m)} / day{static_cast<unsigned>(d)}};
    return static_cast<int>(weekday{date}.iso_encoding());
}

// Returns the ISO 8601 week-based year and week number of a date. The
// Thursday of the same week is always in the week-based year.
inline std::pair<int, int> iso_week(int y, int m, int d) {
    using namespace std::chrono;
    const sys_days date{year{y} / month{static_cast<unsigned>(m)} / day{static_cast<unsigned>(d)}};
    const sys_days thursday = date + days{4 - static_cast<int>(weekday{date}.iso_encoding())};
    const year week_year = year_month_day{thursday}.year();
    const auto week = (thursday - sys_days{week_year / January / 1}).count() / 7 + 1;
    return {static_cast<int>(week_year), static_cast<int>(week)};
}

#endif
