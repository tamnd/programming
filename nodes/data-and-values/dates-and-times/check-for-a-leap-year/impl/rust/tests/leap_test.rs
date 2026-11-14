// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

use algo_check_for_a_leap_year::is_leap_year;

const CASES: &[(&str, i64, bool)] = &[
    ("divisible-by-4", 2024, true),
    ("not-divisible-by-4", 2023, false),
    ("century", 1900, false),
    ("divisible-by-400", 2000, true),
    ("next-century", 2100, false),
    ("year-1600", 1600, true),
    ("year-1700", 1700, false),
    ("year-2400", 2400, true),
    ("year-1", 1, false),
    ("year-4", 4, true),
    ("year-100", 100, false),
    ("year-1500-julian-leap", 1500, false),
    ("year-1582", 1582, false),
    ("year-9999", 9999, false),
    ("year-zero", 0, true),
    ("year-minus-1", -1, false),
    ("year-minus-4", -4, true),
    ("year-minus-100", -100, false),
    ("year-minus-400", -400, true),
];

#[test]
fn cases() {
    for &(name, year, want) in CASES {
        assert_eq!(is_leap_year(year), want, "{name}");
    }
}

#[test]
fn leap_days_in_400_years() {
    assert_eq!((2000..2400).filter(|&y| is_leap_year(y)).count(), 97);
}
