// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

use algo_compute_fibonacci_numbers::{checked_fibonacci, fibonacci};

const CASES: &[(&str, i64, i64)] = &[
    ("ten", 10, 55),
    ("twenty", 20, 6765),
    ("thirty", 30, 832040),
    ("zero", 0, 0),
    ("one", 1, 1),
    ("two", 2, 1),
    ("largest-32-bit", 46, 1836311903),
    ("beyond-32-bit", 47, 2971215073),
    ("binet-fails", 71, 308061521170129),
    ("largest-64-bit", 92, 7540113804746346429),
];

#[test]
fn cases() {
    for &(name, n, want) in CASES {
        assert_eq!(fibonacci(n), want, "{name}");
    }
}

#[test]
fn overflow_is_none() {
    assert_eq!(checked_fibonacci(92), Some(7540113804746346429));
    assert_eq!(checked_fibonacci(93), None);
}
