// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

use algo_declare_a_constant::{seconds_in, SECONDS_PER_DAY};

const CASES: &[(&str, i64, i64)] = &[
    ("one-day", 1, 86400),
    ("one-week", 7, 604800),
    ("one-year", 365, 31536000),
    ("zero", 0, 0),
    ("two-days", 2, 172800),
    ("leap-year", 366, 31622400),
    ("last-32-bit-day", 24855, 2147472000),
    ("past-32-bits", 24856, 2147558400),
    ("million-days", 1000000, 86400000000),
];

#[test]
fn cases() {
    for &(name, days, want) in CASES {
        assert_eq!(seconds_in(days), want, "{name}");
    }
}

#[test]
fn seconds_per_day() {
    assert_eq!(SECONDS_PER_DAY, 86_400);
}
