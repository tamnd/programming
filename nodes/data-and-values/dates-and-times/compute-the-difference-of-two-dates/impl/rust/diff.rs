// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

/// Returns the number of days from 1970-01-01 to a date of the proleptic
/// Gregorian calendar. The formula is from Howard Hinnant, "chrono-Compatible
/// Low-Level Date Algorithms".
pub fn days_from_civil(year: i64, month: i64, day: i64) -> i64 {
    let y = if month <= 2 { year - 1 } else { year };
    let era = y.div_euclid(400);
    let yoe = y - era * 400;
    let mp = (month + 9) % 12;
    let doy = (153 * mp + 2) / 5 + day - 1;
    let doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    era * 146_097 + doe - 719_468
}

/// Returns the number of days from the start date to the end date.
pub fn days_between(
    start_year: i64,
    start_month: i64,
    start_day: i64,
    end_year: i64,
    end_month: i64,
    end_day: i64,
) -> i64 {
    days_from_civil(end_year, end_month, end_day) - days_from_civil(start_year, start_month, start_day)
}
