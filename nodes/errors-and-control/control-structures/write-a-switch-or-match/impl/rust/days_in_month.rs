// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

/// Returns the number of days in a month of a common year, or 0 for a bad month.
pub fn days_in_month(month: i32) -> i32 {
    // match must cover every i32, so the last arm is _.
    match month {
        2 => 28,
        4 | 6 | 9 | 11 => 30,
        1 | 3 | 5 | 7 | 8 | 10 | 12 => 31,
        _ => 0,
    }
}
