// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

use algo_return_multiple_values::div_mod;

const CASES: &[(&str, i32, i32, i32, i32)] = &[
    ("positive", 17, 5, 3, 2),
    ("exact", 20, 4, 5, 0),
    ("smaller-dividend", 3, 10, 0, 3),
    ("negative-dividend", -17, 5, -4, 3),
    ("negative-divisor", 17, -5, -4, -3),
    ("both-negative", -17, -5, 3, -2),
    ("exact-negative", -20, 4, -5, 0),
    ("small-negative", -3, 10, -1, 7),
    ("zero-dividend", 0, 7, 0, 0),
    ("zero-negative-divisor", 0, -7, 0, 0),
    ("divisor-one", 9, 1, 9, 0),
    ("int-max", i32::MAX, 2, 1073741823, 1),
    ("int-min", i32::MIN, 3, -715827883, 1),
    ("min-by-max", i32::MIN, i32::MAX, -2, 2147483646),
    ("max-by-min", i32::MAX, i32::MIN, -1, -1),
];

#[test]
fn cases() {
    for &(name, a, b, q, r) in CASES {
        let (quotient, remainder) = div_mod(a, b);
        assert_eq!((quotient, remainder), (q, r), "{name}");
    }
}

#[test]
fn same_as_div_euclid_for_a_positive_divisor() {
    for &(_, a, b, _, _) in CASES {
        if b > 0 {
            assert_eq!(div_mod(a, b), (a.div_euclid(b), a.rem_euclid(b)));
        }
    }
}
