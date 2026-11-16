// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

use algo_define_a_function::clamp;

const CASES: &[(&str, i32, i32, i32, i32)] = &[
    ("inside", 5, 1, 10, 5),
    ("below", -3, 0, 10, 0),
    ("above", 42, 0, 10, 10),
    ("at-low", 0, 0, 10, 0),
    ("at-high", 10, 0, 10, 10),
    ("equal-bounds", 7, 3, 3, 3),
    ("negative-range", -50, -20, -10, -20),
    ("zero", 0, -5, 5, 0),
    ("int-max", i32::MAX, -100, 100, 100),
    ("int-min", i32::MIN, -100, 100, -100),
    ("full-range", i32::MIN, i32::MIN, i32::MAX, i32::MIN),
];

#[test]
fn cases() {
    for &(name, value, low, high, want) in CASES {
        assert_eq!(clamp(value, low, high), want, "{name}");
    }
}

#[test]
fn same_as_ord_clamp() {
    for &(_, value, low, high, _) in CASES {
        assert_eq!(clamp(value, low, high), value.clamp(low, high));
    }
}
