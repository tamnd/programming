// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

use algo_get_the_current_date_and_time::{now_utc, system_ns, utc_timestamp};

const CASES: &[(&str, i64, &str)] = &[
    ("epoch", 0, "1970-01-01T00:00:00.000Z"),
    ("billennium", 1000000000000000000, "2001-09-09T01:46:40.000Z"),
    ("with-milliseconds", 1710054000123456789, "2024-03-10T07:00:00.123Z"),
    ("leap-day", 1709208000000000000, "2024-02-29T12:00:00.000Z"),
    ("truncate-not-round", 999999, "1970-01-01T00:00:00.000Z"),
    ("one-ns-before-epoch", -1, "1969-12-31T23:59:59.999Z"),
    ("one-ms-before-epoch", -1000000, "1969-12-31T23:59:59.999Z"),
    ("just-over-one-ms-before-epoch", -1000001, "1969-12-31T23:59:59.998Z"),
    ("last-ms-of-a-year", 1704067199999999999, "2023-12-31T23:59:59.999Z"),
    ("year-2000-is-leap", 951782400000000000, "2000-02-29T00:00:00.000Z"),
    ("year-2100-is-not-leap", 4107542399999999999, "2100-02-28T23:59:59.999Z"),
    ("after-int32-seconds", 2147483648000000000, "2038-01-19T03:14:08.000Z"),
    ("int64-max", 9223372036854775807, "2262-04-11T23:47:16.854Z"),
    ("int64-min", i64::MIN, "1677-09-21T00:12:43.145Z"),
];

#[test]
fn utc_timestamp_cases() {
    for &(name, epoch_ns, want) in CASES {
        assert_eq!(utc_timestamp(epoch_ns), want, "{name}");
    }
}

#[test]
fn now_utc_with_a_fake_clock() {
    let mut calls = 0;
    let got = now_utc(|| {
        calls += 1;
        1_710_054_000_123_456_789
    });
    assert_eq!(got, "2024-03-10T07:00:00.123Z");
    assert_eq!(calls, 1);
}

#[test]
fn now_utc_with_the_real_clock() {
    let got = now_utc(system_ns);
    let b = got.as_bytes();
    assert_eq!(b.len(), 24, "{got}");
    assert!(b[10] == b'T' && b[19] == b'.' && b[23] == b'Z', "{got}");
    assert!(got[..4].parse::<i32>().unwrap() >= 2024, "{got}");
}
