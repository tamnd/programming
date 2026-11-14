// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

use algo_get_a_unix_timestamp::{days_from_civil, now_unix_millis, now_unix_seconds, unix_timestamp};

#[allow(clippy::type_complexity)]
const CASES: &[(&str, i64, i64, i64, i64, i64, i64, i64, i64)] = &[
    ("epoch", 1970, 1, 1, 0, 0, 0, 0, 0),
    ("billennium", 2001, 9, 9, 1, 46, 40, 0, 1000000000),
    ("negative-offset", 2024, 3, 10, 2, 0, 0, -300, 1710054000),
    ("positive-offset", 2024, 3, 10, 16, 0, 0, 540, 1710054000),
    ("one-second-before-epoch", 1969, 12, 31, 23, 59, 59, 0, -1),
    ("offset-moves-before-epoch", 1970, 1, 1, 0, 0, 0, 60, -3600),
    ("offset-moves-to-next-year", 2023, 12, 31, 19, 0, 0, -300, 1704067200),
    ("offset-of-45-minutes", 2024, 1, 1, 5, 45, 0, 345, 1704067200),
    ("offset-of-minus-9-30", 2023, 12, 31, 14, 30, 0, -570, 1704067200),
    ("offset-plus-14", 1970, 1, 1, 14, 0, 0, 840, 0),
    ("offset-minus-12", 1969, 12, 31, 12, 0, 0, -720, 0),
    ("leap-day", 2024, 2, 29, 0, 0, 0, 0, 1709164800),
    ("year-2000-is-leap", 2000, 2, 29, 0, 0, 0, 0, 951782400),
    ("year-1900-is-not-leap", 1900, 3, 1, 0, 0, 0, 0, -2203891200),
    ("int32-max", 2038, 1, 19, 3, 14, 7, 0, 2147483647),
    ("after-int32-max", 2038, 1, 19, 3, 14, 8, 0, 2147483648),
    ("int32-min", 1901, 12, 13, 20, 45, 52, 0, -2147483648),
    ("year-99-is-not-1999", 99, 12, 31, 23, 59, 59, 0, -59011459201),
    ("first-second-of-year-1", 1, 1, 1, 0, 0, 0, 0, -62135596800),
    ("last-second-of-year-9999", 9999, 12, 31, 23, 59, 59, 0, 253402300799),
];

#[test]
fn cases() {
    for &(name, year, month, day, hour, minute, second, offset, want) in CASES {
        assert_eq!(unix_timestamp(year, month, day, hour, minute, second, offset), want, "{name}");
    }
}

#[test]
fn days_from_civil_known_dates() {
    assert_eq!(days_from_civil(1970, 1, 1), 0);
    assert_eq!(days_from_civil(1969, 12, 31), -1);
    assert_eq!(days_from_civil(2000, 3, 1) - days_from_civil(2000, 2, 28), 2);
    assert_eq!(days_from_civil(1900, 3, 1) - days_from_civil(1900, 2, 28), 1);
}

#[test]
fn now() {
    let seconds = now_unix_seconds();
    let millis = now_unix_millis();
    assert!(seconds >= 1_704_067_200);
    let diff = millis.div_euclid(1_000) - seconds;
    assert!((0..=1).contains(&diff));
}
