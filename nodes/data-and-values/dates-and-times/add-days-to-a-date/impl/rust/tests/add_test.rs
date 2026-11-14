// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

use algo_add_days_to_a_date::{add_to_date, civil_from_days, days_from_civil, days_in_month};

#[allow(clippy::type_complexity)]
const CASES: &[(&str, i64, i64, i64, i64, i64, i64, &str)] = &[
    ("one-day", 2024, 3, 10, 0, 0, 1, "2024-03-11"),
    ("one-month", 2024, 1, 15, 0, 1, 0, "2024-02-15"),
    ("one-year", 2023, 6, 1, 1, 0, 0, "2024-06-01"),
    ("day-into-next-year", 2023, 12, 31, 0, 0, 1, "2024-01-01"),
    ("nothing-to-add", 2024, 2, 29, 0, 0, 0, "2024-02-29"),
    ("month-end-to-leap-february", 2024, 1, 31, 0, 1, 0, "2024-02-29"),
    ("month-end-to-short-february", 2023, 1, 31, 0, 1, 0, "2023-02-28"),
    ("month-end-to-30-day-month", 2024, 3, 31, 0, 1, 0, "2024-04-30"),
    ("back-from-month-end", 2024, 5, 31, 0, -1, 0, "2024-04-30"),
    ("back-to-leap-february", 2024, 3, 31, 0, -1, 0, "2024-02-29"),
    ("leap-day-plus-one-year", 2024, 2, 29, 1, 0, 0, "2025-02-28"),
    ("leap-day-plus-four-years", 2024, 2, 29, 4, 0, 0, "2028-02-29"),
    ("year-and-month-at-once", 2024, 2, 29, 1, 1, 0, "2025-03-29"),
    ("months-before-days", 2024, 1, 31, 0, 1, 1, "2024-03-01"),
    ("month-into-next-year", 2023, 12, 15, 0, 1, 0, "2024-01-15"),
    ("thirteen-months-back", 2024, 1, 15, 0, -13, 0, "2022-12-15"),
    ("day-back-into-last-year", 2024, 1, 1, 0, 0, -1, "2023-12-31"),
    ("year-1900-is-not-leap", 1900, 2, 28, 0, 0, 1, "1900-03-01"),
    ("year-2100-is-not-leap", 2096, 2, 29, 4, 0, 0, "2100-02-28"),
    ("many-days", 1970, 1, 1, 0, 0, 2932896, "9999-12-31"),
    ("many-days-back", 9999, 12, 31, 0, 0, -3652058, "0001-01-01"),
    ("many-months", 1, 1, 31, 0, 119987, 0, "9999-12-31"),
];

#[test]
fn cases() {
    for &(name, year, month, day, years, months, days, want) in CASES {
        assert_eq!(add_to_date(year, month, day, years, months, days), want, "{name}");
    }
}

#[test]
fn round_trip_of_days() {
    for days in [-719_162, -1, 0, 1, 19_782, 2_932_896] {
        let (y, m, d) = civil_from_days(days);
        assert_eq!(days_from_civil(y, m, d), days);
    }
}

#[test]
fn month_lengths() {
    assert_eq!(days_in_month(2024, 2), 29);
    assert_eq!(days_in_month(1900, 2), 28);
    assert_eq!(days_in_month(2000, 2), 29);
    assert_eq!(days_in_month(2024, 4), 30);
}
