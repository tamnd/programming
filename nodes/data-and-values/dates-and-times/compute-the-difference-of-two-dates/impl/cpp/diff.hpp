// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

#ifndef DAYS_BETWEEN_HPP
#define DAYS_BETWEEN_HPP

#include <chrono>

// Returns the number of days from the start date to the end date. sys_days
// counts whole days from 1970-01-01, so the difference is a count of days.
inline long long days_between(int start_year, int start_month, int start_day, int end_year, int end_month, int end_day) {
    using namespace std::chrono;
    const sys_days start{year{start_year} / month{static_cast<unsigned>(start_month)} / day{static_cast<unsigned>(start_day)}};
    const sys_days end{year{end_year} / month{static_cast<unsigned>(end_month)} / day{static_cast<unsigned>(end_day)}};
    return (end - start).count();
}

#endif
