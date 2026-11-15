// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

use algo_loop_with_an_index::positions;

#[test]
fn cases() {
    let cases: &[(&str, &[&str], &str, &[usize])] = &[
        ("two-matches", &["a", "b", "a", "c"], "a", &[0, 2]),
        ("words", &["red", "green", "blue", "green"], "green", &[1, 3]),
        ("empty", &[], "a", &[]),
        ("none", &["a", "b"], "z", &[]),
        ("all", &["x", "x", "x"], "x", &[0, 1, 2]),
        ("last", &["a", "b", "c"], "c", &[2]),
        ("case", &["A", "a"], "a", &[1]),
        ("empty-string", &["", "a", ""], "", &[0, 2]),
    ];
    for &(name, items, target, want) in cases {
        assert_eq!(positions(items, target), want, "{name}");
    }
}
