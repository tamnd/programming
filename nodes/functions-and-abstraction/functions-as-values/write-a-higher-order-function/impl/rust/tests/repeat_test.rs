// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

use algo_write_a_higher_order_function::{collatz_after, collatz_step, repeat};

const CASES: &[(&str, i32, i32, i32)] = &[
    ("one-step", 6, 1, 3),
    ("three-steps", 6, 3, 5),
    ("ten-steps", 27, 10, 214),
    ("zero-times", 7, 0, 7),
    ("start-one", 1, 1, 4),
    ("cycle", 1, 3, 1),
    ("past-one", 16, 10, 1),
    ("peak-27", 27, 77, 9232),
    ("peak-9663", 9663, 48, 27114424),
    ("limit", 10000, 1000, 2),
];

#[test]
fn cases() {
    for &(name, start, times, want) in CASES {
        assert_eq!(collatz_after(start, times), want, "{name}");
    }
}

#[test]
fn repeat_takes_a_closure() {
    assert_eq!(repeat(|x| x * 2, 10)(1), 1024);
}

#[test]
fn repeat_zero_times_is_the_identity() {
    assert_eq!(repeat(collatz_step, 0)(27), 27);
}
