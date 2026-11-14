// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

/// Returns true for a leap year of the Gregorian calendar.
pub fn is_leap(year: i64) -> bool {
    year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)
}

/// Returns the number of days in a month (1 to 12) of a year.
pub fn days_in_month(year: i64, month: i64) -> i64 {
    match month {
        2 if is_leap(year) => 29,
        2 => 28,
        4 | 6 | 9 | 11 => 30,
        _ => 31,
    }
}

/// Returns the number of days from 1970-01-01 to a date (Howard Hinnant).
pub fn days_from_civil(year: i64, month: i64, day: i64) -> i64 {
    let y = if month <= 2 { year - 1 } else { year };
    let era = y.div_euclid(400);
    let yoe = y - era * 400;
    let mp = (month + 9) % 12;
    let doy = (153 * mp + 2) / 5 + day - 1;
    let doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    era * 146_097 + doe - 719_468
}

/// Returns the date of a number of days from 1970-01-01 (Howard Hinnant).
pub fn civil_from_days(days: i64) -> (i64, i64, i64) {
    let z = days + 719_468;
    let era = z.div_euclid(146_097);
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1_460 + doe / 36_524 - doe / 146_096) / 365;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let day = doy - (153 * mp + 2) / 5 + 1;
    let month = if mp < 10 { mp + 3 } else { mp - 9 };
    let year = yoe + era * 400 + i64::from(month <= 2);
    (year, month, day)
}

/// Adds years and months at once, then days, and returns the date as
/// YYYY-MM-DD. A day after the end of the new month becomes its last day.
pub fn add_to_date(year: i64, month: i64, day: i64, years: i64, months: i64, days: i64) -> String {
    let total = year * 12 + (month - 1) + years * 12 + months;
    let new_year = total.div_euclid(12);
    let new_month = total.rem_euclid(12) + 1;
    let new_day = day.min(days_in_month(new_year, new_month));
    let (y, m, d) = civil_from_days(days_from_civil(new_year, new_month, new_day) + days);
    format!("{y:04}-{m:02}-{d:02}")
}
