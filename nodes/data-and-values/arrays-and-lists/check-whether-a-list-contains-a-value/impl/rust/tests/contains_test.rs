// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

use algo_check_whether_a_list_contains_a_value::contains;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], i32, bool)] = &[
        ("present", &[1, 2, 3], 2, true),
        ("absent", &[1, 2, 3], 4, false),
        ("empty", &[], 0, false),
        ("single-yes", &[5], 5, true),
        ("single-no", &[5], 0, false),
        ("first", &[7, 8, 9], 7, true),
        ("last", &[7, 8, 9], 9, true),
        ("duplicates", &[2, 2, 2], 2, true),
        ("negative", &[-1, -2, -3], -2, true),
        ("zero-absent", &[-1, 1], 0, false),
        ("limit", &[2147483647, i32::MIN], i32::MIN, true),
        ("limit-absent", &[2147483647], i32::MIN, false),
    ];
    for &(name, items, value, want) in cases {
        assert_eq!(contains(items, value), want, "{name}");
    }
}
