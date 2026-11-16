// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

// A const item needs a type, and the compiler computes its value. Each use
// of the name gets the value.
const SECONDS_PER_MINUTE: i64 = 60;
const MINUTES_PER_HOUR: i64 = 60;
const HOURS_PER_DAY: i64 = 24;
pub const SECONDS_PER_DAY: i64 = SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY;

/// Returns the number of seconds in `days` days of 24 hours.
pub fn seconds_in(days: i64) -> i64 {
    days * SECONDS_PER_DAY
}
