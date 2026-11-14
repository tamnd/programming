// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

use std::time::{SystemTime, UNIX_EPOCH};

/// Returns the number of days from 1970-01-01 to a date in the proleptic
/// Gregorian calendar. This is the days_from_civil algorithm of Howard Hinnant.
pub fn days_from_civil(year: i64, month: i64, day: i64) -> i64 {
    let y = if month <= 2 { year - 1 } else { year };
    let era = y.div_euclid(400);
    let yoe = y - era * 400; // 0 to 399
    let mp = (month + 9) % 12; // March is 0
    let doy = (153 * mp + 2) / 5 + day - 1; // 0 to 365
    let doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // 0 to 146096
    era * 146_097 + doe - 719_468
}

/// Returns the seconds since 1970-01-01T00:00:00Z of a local time with the
/// given UTC offset in minutes.
pub fn unix_timestamp(
    year: i64,
    month: i64,
    day: i64,
    hour: i64,
    minute: i64,
    second: i64,
    offset_minutes: i64,
) -> i64 {
    days_from_civil(year, month, day) * 86_400 + hour * 3_600 + (minute - offset_minutes) * 60 + second
}

/// Returns the current Unix time in whole milliseconds, rounded toward the
/// past. A system clock set before 1970 gives a negative value.
pub fn now_unix_millis() -> i64 {
    match SystemTime::now().duration_since(UNIX_EPOCH) {
        Ok(after) => after.as_millis() as i64,
        Err(before) => -(before.duration().as_nanos().div_ceil(1_000_000) as i64),
    }
}

/// Returns the current Unix time in whole seconds, rounded toward the past.
pub fn now_unix_seconds() -> i64 {
    now_unix_millis().div_euclid(1_000)
}
