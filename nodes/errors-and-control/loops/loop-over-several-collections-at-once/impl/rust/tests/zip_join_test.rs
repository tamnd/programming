// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

use algo_loop_over_several_collections_at_once::zip_join;

#[test]
fn cases() {
    let cases: &[(&str, &[&str], &[&str], &[i32], &[&str])] = &[
        ("rosetta", &["a", "b", "c"], &["A", "B", "C"], &[1, 2, 3], &["aA1", "bB2", "cC3"]),
        ("words", &["x", "y"], &["X", "Y"], &[10, 20], &["xX10", "yY20"]),
        ("shorter-first", &["a"], &["A", "B"], &[1, 2, 3], &["aA1"]),
        ("shorter-last", &["a", "b", "c"], &["A", "B", "C"], &[1], &["aA1"]),
        ("one-empty", &["a", "b"], &[], &[1, 2], &[]),
        ("all-empty", &[], &[], &[], &[]),
        ("negative", &["a"], &["A"], &[-5], &["aA-5"]),
        ("empty-strings", &["", "b"], &["A", ""], &[1, 2], &["A1", "b2"]),
    ];
    for &(name, lower, upper, digits, want) in cases {
        assert_eq!(zip_join(lower, upper, digits), want, "{name}");
    }
}
