// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

use algo_find_the_largest_element::largest;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], i32)] = &[
        ("digits", &[3, 1, 4, 1, 5], 5),
        ("middle", &[10, 20, 15], 20),
        ("single", &[7], 7),
        ("all-negative", &[-3, -1, -2], -1),
        ("first", &[9, 2, 3], 9),
        ("last", &[1, 2, 9], 9),
        ("duplicates", &[5, 9, 9, 1], 9),
        ("all-same", &[4, 4, 4], 4),
        ("zero", &[-5, 0, -1], 0),
        ("extremes", &[-2147483648, 2147483647, 0], 2147483647),
        ("min-only", &[-2147483648], -2147483648),
    ];
    for &(name, items, want) in cases {
        assert_eq!(largest(items), want, "{name}");
    }
}

#[test]
#[should_panic]
fn empty_panics() {
    largest(&[]);
}
