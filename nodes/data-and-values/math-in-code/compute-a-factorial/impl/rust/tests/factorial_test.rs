// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

use algo_compute_a_factorial::{checked_factorial, factorial};

const CASES: &[(&str, i64, i64)] = &[
    ("five", 5, 120),
    ("ten", 10, 3628800),
    ("zero", 0, 1),
    ("one", 1, 1),
    ("largest-32-bit", 12, 479001600),
    ("beyond-32-bit", 13, 6227020800),
    ("largest-64-bit", 20, 2432902008176640000),
];

#[test]
fn cases() {
    for &(name, n, want) in CASES {
        assert_eq!(factorial(n), want, "{name}");
        assert_eq!(checked_factorial(n), Some(want), "{name}");
    }
}

#[test]
fn overflow_is_none() {
    assert_eq!(checked_factorial(21), None);
}
