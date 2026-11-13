// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

use algo_sort_with_a_custom_key::sort_by_length;

#[test]
fn cases() {
    let cases: Vec<(&str, &[&str], Vec<&str>)> = vec![
        ("fruits", &["pear", "fig", "apple", "kiwi"], vec!["fig", "kiwi", "pear", "apple"]),
        ("three", &["bb", "a", "ccc"], vec!["a", "bb", "ccc"]),
        ("empty", &[], Vec::<&str>::new()),
        ("single", &["x"], vec!["x"]),
        ("same-length", &["dog", "cat", "ant"], vec!["ant", "cat", "dog"]),
        ("duplicates", &["b", "a", "b"], vec!["a", "b", "b"]),
        ("sorted", &["a", "bb", "ccc"], vec!["a", "bb", "ccc"]),
        ("reverse", &["ccc", "bb", "a"], vec!["a", "bb", "ccc"]),
        ("long", &["abcdefghij", "abcdefghi", "z"], vec!["z", "abcdefghi", "abcdefghij"]),
        ("prefix", &["ab", "a", "abc", "b"], vec!["a", "b", "ab", "abc"]),
    ];
    for (name, words, want) in cases {
        assert_eq!(sort_by_length(words), want, "{name}");
    }
}
