// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

use algo_concatenate_two_lists::concat;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], &[i32], &[i32])] = &[
        ("two-pairs", &[1, 2], &[3, 4], &[1, 2, 3, 4]),
        ("uneven", &[5], &[6, 7, 8], &[5, 6, 7, 8]),
        ("both-empty", &[], &[], &[]),
        ("left-empty", &[], &[1, 2], &[1, 2]),
        ("right-empty", &[1, 2], &[], &[1, 2]),
        ("duplicates", &[1, 1], &[1], &[1, 1, 1]),
        ("order", &[3, 1], &[2], &[3, 1, 2]),
        ("limit", &[2147483647], &[i32::MIN], &[2147483647, i32::MIN]),
    ];
    for &(name, a, b, want) in cases {
        assert_eq!(concat(a, b), want, "{name}");
    }
}
