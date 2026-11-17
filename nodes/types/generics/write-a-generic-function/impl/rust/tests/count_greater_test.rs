// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

use algo_write_a_generic_function::count_greater;

const CASES: &[(&str, &[i32], i32, usize)] = &[
    ("some", &[34, 50, 25, 100, 65], 50, 2),
    ("all", &[3, 4, 5], 2, 3),
    ("none", &[3, 4, 5], 9, 0),
    ("empty", &[], 0, 0),
    ("equal-not-counted", &[5, 5, 5], 5, 0),
    ("duplicates", &[7, 1, 7, 7], 6, 3),
    ("negative", &[-3, -1, 0, 2], -2, 3),
    ("min-limit", &[i32::MIN, 0, i32::MAX], i32::MIN, 2),
    ("max-limit", &[i32::MAX, 1], i32::MAX, 0),
    ("single", &[1], 0, 1),
];

#[test]
fn count_greater_cases() {
    for &(name, items, limit, want) in CASES {
        assert_eq!(count_greater(items, &limit), want, "{name}");
    }
}

#[test]
fn strings() {
    assert_eq!(count_greater(&["pear", "apple", "plum"], &"orange"), 2);
}
