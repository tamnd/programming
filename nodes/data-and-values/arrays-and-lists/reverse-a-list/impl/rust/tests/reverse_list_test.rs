// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

use algo_reverse_a_list::reverse;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], &[i32])] = &[
        ("three", &[1, 2, 3], &[3, 2, 1]),
        ("four", &[4, 7, 1, 9], &[9, 1, 7, 4]),
        ("empty", &[], &[]),
        ("single", &[5], &[5]),
        ("two", &[1, 2], &[2, 1]),
        ("duplicates", &[1, 1, 2], &[2, 1, 1]),
        ("palindrome", &[1, 2, 1], &[1, 2, 1]),
        ("negative", &[-1, 0, -3], &[-3, 0, -1]),
        ("limit", &[2147483647, i32::MIN, 0], &[0, i32::MIN, 2147483647]),
    ];
    for &(name, items, want) in cases {
        assert_eq!(reverse(items), want, "{name}");
    }
}
