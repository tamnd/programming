// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

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

/// Returns the year of a day number. This is the year part of the
/// civil_from_days algorithm of Howard Hinnant.
fn year_from_days(days: i64) -> i64 {
    let z = days + 719_468;
    let era = z.div_euclid(146_097);
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let month = if mp < 10 { mp + 3 } else { mp - 9 };
    yoe + era * 400 + if month <= 2 { 1 } else { 0 }
}

/// Returns the ISO 8601 day of the week of a day number. 1970-01-01 was a
/// Thursday, which is day 4.
fn weekday_from_days(days: i64) -> i64 {
    (days + 3).rem_euclid(7) + 1
}

/// Returns the ISO 8601 day of the week: 1 for Monday to 7 for Sunday.
pub fn day_of_week(year: i64, month: i64, day: i64) -> i64 {
    weekday_from_days(days_from_civil(year, month, day))
}

/// Returns the ISO 8601 week-based year and week number of a date. The
/// Thursday of the same week is always in the week-based year.
pub fn iso_week(year: i64, month: i64, day: i64) -> (i64, i64) {
    let days = days_from_civil(year, month, day);
    let thursday = days + 4 - weekday_from_days(days);
    let week_year = year_from_days(thursday);
    (week_year, (thursday - days_from_civil(week_year, 1, 1)) / 7 + 1)
}
