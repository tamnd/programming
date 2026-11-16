// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

use algo_take_the_first_n_items::{take, take_naturals};

#[test]
fn cases() {
    let cases: Vec<(&str, &[i32], usize, Vec<i32>)> = vec![
        ("first-three", &[1, 2, 3, 4, 5], 3, vec![1, 2, 3]),
        ("first-two", &[10, 20, 30, 40], 2, vec![10, 20]),
        ("zero", &[1, 2], 0, vec![]),
        ("empty", &[], 3, vec![]),
        ("more-than-length", &[1, 2], 5, vec![1, 2]),
        ("equal-length", &[4, 5, 6], 3, vec![4, 5, 6]),
        ("one", &[9, 8], 1, vec![9]),
        ("negative", &[-1, -2, -3], 2, vec![-1, -2]),
        ("extremes", &[-2147483648, 2147483647, 0], 2, vec![-2147483648, 2147483647]),
    ];
    for (name, items, n, want) in cases {
        assert_eq!(take(items, n), want, "{name}");
    }
}

#[test]
fn infinite() {
    let cases: Vec<(&str, i32, usize, Vec<i32>)> = vec![
        ("from-zero", 0, 5, vec![0, 1, 2, 3, 4]),
        ("from-ten", 10, 3, vec![10, 11, 12]),
        ("none", 7, 0, vec![]),
        ("one", 0, 1, vec![0]),
        ("large-start", 999998, 2, vec![999998, 999999]),
    ];
    for (name, start, n, want) in cases {
        assert_eq!(take_naturals(start, n), want, "{name}");
    }
}
