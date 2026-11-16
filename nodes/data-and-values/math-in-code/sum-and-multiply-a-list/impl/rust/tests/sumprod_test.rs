// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

use algo_sum_and_multiply_a_list::{checked_sum_and_product, sum_and_product};

const CASES: &[(&str, &[i64], (i64, i64))] = &[
    ("small", &[1, 2, 3, 4], (10, 24)),
    ("mixed", &[3, -2, 5], (6, -30)),
    ("empty", &[], (0, 1)),
    ("single", &[7], (7, 7)),
    ("with-zero", &[5, 0, 9], (14, 0)),
    ("two-negatives", &[-2, -3], (-5, 6)),
    ("three-negatives", &[-1, -2, -3], (-6, -6)),
    ("ones", &[1, 1, 1], (3, 1)),
    ("large-product", &[3037000499, 3037000499], (6074000998, 9223372030926249001)),
    ("to-the-limit", &[9223372036854775806, 1], (9223372036854775807, 9223372036854775806)),
    ("beyond-double", &[9007199254740993], (9007199254740993, 9007199254740993)),
    ("min", &[i64::MIN], (i64::MIN, i64::MIN)),
];

#[test]
fn cases() {
    for &(name, items, want) in CASES {
        assert_eq!(sum_and_product(items), want, "{name}");
        assert_eq!(checked_sum_and_product(items), Some(want), "{name}");
    }
}

#[test]
fn overflow_is_none() {
    assert_eq!(checked_sum_and_product(&[1 << 32, 1 << 32]), None);
}
