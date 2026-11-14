// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

//! Reads the current date and time and writes it in UTC.

use std::time::{SystemTime, UNIX_EPOCH};

/// Returns the year, month and day of a count of days since 1970-01-01, in
/// the proleptic Gregorian calendar. This is the `civil_from_days` algorithm
/// of Howard Hinnant: it counts in eras of 400 years that start on March 1.
fn civil_from_days(days: i64) -> (i64, u32, u32) {
    let z = days + 719_468;
    let era = z.div_euclid(146_097);
    let doe = z.rem_euclid(146_097); // [0, 146096]
    let yoe = (doe - doe / 1_460 + doe / 36_524 - doe / 146_096) / 365; // [0, 399]
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
    let mp = (5 * doy + 2) / 153; // [0, 11], March is 0
    let day = (doy - (153 * mp + 2) / 5 + 1) as u32;
    let month = if mp < 10 { mp + 3 } else { mp - 9 } as u32;
    let year = yoe + era * 400 + i64::from(month <= 2);
    (year, month, day)
}

/// Returns the UTC date and time of `epoch_ns` as "YYYY-MM-DDTHH:MM:SS.mmmZ".
pub fn utc_timestamp(epoch_ns: i64) -> String {
    // div_euclid goes toward the past for a positive divisor. The / of Rust goes toward zero.
    let ms = epoch_ns.div_euclid(1_000_000);
    let secs = ms.div_euclid(1_000);
    let days = secs.div_euclid(86_400);
    let sod = secs.rem_euclid(86_400);
    let (year, month, day) = civil_from_days(days);
    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        sod / 3_600,
        sod / 60 % 60,
        sod % 60,
        ms.rem_euclid(1_000)
    )
}

/// The real clock: `SystemTime::now()` in nanoseconds since the Unix epoch.
/// `duration_since` returns an error for a time before 1970.
pub fn system_ns() -> i64 {
    match SystemTime::now().duration_since(UNIX_EPOCH) {
        Ok(d) => i64::try_from(d.as_nanos()).unwrap_or(i64::MAX),
        Err(e) => i64::try_from(e.duration().as_nanos()).map_or(i64::MIN, |n| -n),
    }
}

/// Reads the clock once and returns the UTC date and time of the reading.
pub fn now_utc(mut clock: impl FnMut() -> i64) -> String {
    utc_timestamp(clock())
}
