// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

use algo_create_a_set::toggle;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<&str>, Vec<&str>)> = vec![
        ("colors", vec!["red", "blue", "red", "green"], vec!["blue", "green"]),
        ("fresh", vec!["b", "a"], vec!["a", "b"]),
        ("empty", vec![], vec![]),
        ("two-times", vec!["a", "a"], vec![]),
        ("three-times", vec!["a", "a", "a"], vec!["a"]),
        ("case", vec!["a", "A"], vec!["A", "a"]),
        ("prefix", vec!["ab", "a"], vec!["a", "ab"]),
    ];
    for (name, words, want) in cases {
        assert_eq!(toggle(&words), want, "{name}");
    }
}
