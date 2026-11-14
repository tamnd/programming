// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

#pragma once

#include <chrono>
#include <format>
#include <string>

// Adds years and months at once, then days, and returns the date as
// YYYY-MM-DD. A day after the end of the new month becomes its last day.
inline std::string add_to_date(int year, int month, int day, int n_years, int n_months, int n_days) {
    namespace chr = std::chrono;
    const chr::year_month_day start{chr::year{year}, chr::month{static_cast<unsigned>(month)},
                                    chr::day{static_cast<unsigned>(day)}};
    // Adding months keeps the day. The result is not ok() for a day such as February 30.
    chr::year_month_day moved = start + chr::months{n_years * 12 + n_months};
    if (!moved.ok()) {
        moved = moved.year() / moved.month() / chr::last;
    }
    const chr::sys_days result = chr::sys_days{moved} + chr::days{n_days};
    return std::format("{:%F}", result);
}
