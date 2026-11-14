// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

use algo_find_the_index_of_a_value::index_of;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], i32, i64)] = &[
        ("middle", &[4, 7, 1], 7, 1),
        ("absent", &[4, 7, 1], 9, -1),
        ("empty", &[], 0, -1),
        ("single", &[5], 5, 0),
        ("single-absent", &[5], 0, -1),
        ("first", &[7, 8, 9], 7, 0),
        ("last", &[7, 8, 9], 9, 2),
        ("duplicates", &[3, 5, 3, 5], 5, 1),
        ("all-same", &[2, 2, 2], 2, 0),
        ("negative", &[0, -1, -2], -2, 2),
        ("limit", &[2147483647, i32::MIN], i32::MIN, 1),
        ("limit-absent", &[2147483647], i32::MIN, -1),
    ];
    for &(name, items, value, want) in cases {
        assert_eq!(index_of(items, value), want, "{name}");
    }
}
