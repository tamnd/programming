// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

use std::cell::RefCell;
use std::time::{Duration, Instant};

use algo_measure_elapsed_time::{elapsed_ms, measure};

const CASES: &[(&str, i64, i64, &str)] = &[
    ("one-and-a-half-ms", 1000000000, 1001500000, "1.500 ms"),
    ("seconds", 0, 2345678901, "2345.678 ms"),
    ("under-one-ms", 50000, 750000, "0.700 ms"),
    ("zero", 123456789, 123456789, "0.000 ms"),
    ("under-one-microsecond", 0, 999, "0.000 ms"),
    ("truncate-not-round", 0, 1999999, "1.999 ms"),
    ("leading-zero-in-fraction", 0, 1005000, "1.005 ms"),
    ("exact-ms", 0, 7000000, "7.000 ms"),
    ("one-hour", 0, 3600000000000, "3600000.000 ms"),
    ("negative-readings", -5000000, -3500000, "1.500 ms"),
    ("across-zero", -1000000, 500000, "1.500 ms"),
    (
        "beyond-2-pow-53",
        9007199254740993,
        9007199255741992,
        "1.000 ms",
    ),
    (
        "near-int64-max",
        9223372035854775807,
        9223372036854775807,
        "1000.000 ms",
    ),
    (
        "near-int64-min",
        i64::MIN,
        -9223372035854775808,
        "1000.000 ms",
    ),
];

#[test]
fn elapsed_ms_cases() {
    for &(name, start_ns, end_ns, want) in CASES {
        assert_eq!(elapsed_ms(start_ns, end_ns), want, "{name}");
    }
}

#[test]
fn measure_with_a_fake_clock() {
    // An Instant cannot be made from a number, so the fake readings are offsets from one real Instant.
    let base = Instant::now();
    let calls = RefCell::new(Vec::new());
    let mut readings = [5_000_000, 6_500_000].into_iter();
    let now = || {
        calls.borrow_mut().push("clock");
        base + Duration::from_nanos(readings.next().unwrap())
    };
    let got = measure(|| calls.borrow_mut().push("work"), now);
    assert_eq!(got, Duration::from_micros(1_500));
    assert_eq!(*calls.borrow(), ["clock", "work", "clock"]);
}

#[test]
fn measure_with_the_monotonic_clock() {
    let mut calls = Vec::new();
    let got = measure(|| calls.push("work"), Instant::now);
    assert!(got >= Duration::ZERO);
    assert_eq!(calls, ["work"]);
}
