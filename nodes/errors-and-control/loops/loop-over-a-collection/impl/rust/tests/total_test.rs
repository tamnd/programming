// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

use algo_loop_over_a_collection::total;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], i64)] = &[
        ("three", &[1, 2, 3], 6),
        ("mixed", &[10, -4, 7, 0, 3], 16),
        ("empty", &[], 0),
        ("single", &[7], 7),
        ("negative", &[-5, 2, -1], -4),
        ("limit", &[i32::MAX, i32::MAX, 2], 4294967296),
        ("limit-low", &[i32::MIN, i32::MIN], -4294967296),
    ];
    for &(name, items, want) in cases {
        assert_eq!(total(items), want, "{name}");
    }
}
