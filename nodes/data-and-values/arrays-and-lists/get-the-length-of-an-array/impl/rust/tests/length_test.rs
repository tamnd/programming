// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

use algo_get_the_length_of_an_array::length;

#[test]
fn cases() {
    let cases: &[(&str, &[&str], usize)] = &[
        ("three", &["a", "b", "c"], 3),
        ("two-words", &["apple", "banana"], 2),
        ("empty", &[], 0),
        ("single", &["x"], 1),
        ("empty-strings", &["", ""], 2),
        ("non-ascii", &["é", "日本語"], 2),
        ("duplicates", &["a", "a", "a"], 3),
        ("ten", &["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"], 10),
    ];
    for &(name, items, want) in cases {
        assert_eq!(length(items), want, "{name}");
    }
}
